#include "pathtracer.h"
#include "../geometry/util.h"
#include "../gui/render.h"

#include <SDL2/SDL.h>
#include <thread>

namespace PT
{

Pathtracer::Pathtracer(Gui::Widget_Render &gui, Vec2 screen_dim) : thread_pool(std::thread::hardware_concurrency()), gui(gui), camera(screen_dim), scene(List<Object>())
{
    accumulator_samples = 0;
    total_epochs = 0;
    completed_epochs = 0;
    out_w = out_h = 0;
    n_samples = 0;
}

Pathtracer::~Pathtracer()
{
    cancel();
    thread_pool.stop();
}

void Pathtracer::build_lights(Scene &layout_scene)
{

    point_lights.clear();
    env_light.reset();

    layout_scene.for_items([&, this](const Scene_Item &item)
                           {
                               if (item.is<Scene_Light>())
                               {

                                   const Scene_Light &light = item.get<Scene_Light>();
                                   Spectrum r = light.radiance();

                                   switch (light.opt.type)
                                   {
                                       case Light_Type::directional:
                                       {
                                           point_lights.push_back(Delta_Light(Directional_Light(r), light.id(), light.pose.transform()));
                                       }
                                           break;
                                       case Light_Type::sphere:
                                       {
                                           if (light.opt.has_emissive_map)
                                           {
                                               env_light = Env_Light(Env_Map(light.emissive_copy()));
                                           } else
                                           {
                                               env_light = Env_Light(Env_Sphere(r));
                                           }
                                       }
                                           break;
                                       case Light_Type::hemisphere:
                                       {
                                           env_light = Env_Light(Env_Hemisphere(r));
                                       }
                                           break;
                                       case Light_Type::point:
                                       {
                                           point_lights.push_back(Delta_Light(Point_Light(r), light.id(), light.pose.transform()));
                                       }
                                           break;
                                       case Light_Type::spot:
                                       {
                                           point_lights.push_back(Delta_Light(Spot_Light(r, light.opt.angle_bounds), light.id(), light.pose.transform()));
                                       }
                                           break;
                                       default:
                                           return;
                                   }
                               }
                           });
}

void Pathtracer::build_scene(Scene &layout_scene)
{

    // It would be nice to let the interface be usable here (as with
    // the path-tracing part), but this would cause too much hassle with
    // editing the scene while building BVHs from it.
    // This could be worked around by first copying all the mesh data
    // and then building the BVHs, but I don't think it's that big
    // of a deal, as BVH building should take at most a few seconds
    // even with many big meshes.

    // We could also do instancing instead of duplicating the bvh
    // for big meshes, but that's something to add in the future

    materials.clear();

    std::vector<std::future<std::vector<Object>>> futures;
    std::vector<Object> area_light_list;

    layout_scene.for_items([&, this](Scene_Item &item)
                           {
                               if (item.is<Scene_Object>())
                               {

                                   Scene_Object &obj = item.get<Scene_Object>();
                                   unsigned int idx = (unsigned int) materials.size();
                                   const Material::Options &opt = obj.material.opt;

                                   if (!obj.opt.render)
                                       return;

                                   switch (opt.type)
                                   {
                                       case Material_Type::lambertian:
                                       {
                                           materials.push_back(BSDF(BSDF_Lambertian(opt.albedo.to_linear())));
                                       }
                                           break;
                                       case Material_Type::mirror:
                                       {
                                           materials.push_back(BSDF(BSDF_Mirror(opt.reflectance)));
                                       }
                                           break;
                                       case Material_Type::refract:
                                       {
                                           materials.push_back(BSDF(BSDF_Refract(opt.transmittance, opt.ior)));
                                       }
                                           break;
                                       case Material_Type::glass:
                                       {
                                           materials.push_back(BSDF(BSDF_Glass(opt.transmittance, opt.reflectance, opt.ior)));
                                       }
                                           break;
                                       case Material_Type::diffuse_light:
                                       {
                                           materials.push_back(BSDF(BSDF_Diffuse(obj.material.emissive())));
                                           // NOTE(max): we use an approximate triangle mesh for shape objects
                                           // because PT::Object only supports sampling triangles
                                           if (obj.is_shape())
                                           {
                                               area_light_list.push_back(Object(Tri_Mesh(obj.opt.shape.mesh(), false), obj.id(), idx, obj.pose.transform()));
                                           } else
                                           {
                                               area_light_list.push_back(Object(Tri_Mesh(obj.posed_mesh(), false), obj.id(), idx, obj.pose.transform()));
                                           }
                                       }
                                           break;
                                       default:
                                           return;
                                   }

                                   bool use_bvh = scene_use_bvh;
                                   futures.push_back(thread_pool.enqueue([&obj, use_bvh, idx]()
                                                                         {
                                                                             std::vector<Object> objs;
                                                                             if (obj.is_shape())
                                                                             {
                                                                                 Shape shape(obj.opt.shape);
                                                                                 objs.emplace_back(std::move(shape), obj.id(), idx, obj.pose.transform());
                                                                             } else
                                                                             {
                                                                                 Tri_Mesh mesh(obj.posed_mesh(), use_bvh);
                                                                                 objs.emplace_back(std::move(mesh), obj.id(), idx, obj.pose.transform());
                                                                             }
                                                                             return objs;
                                                                         }));

                               } else if (item.is<Scene_Particles>())
                               {

                                   Scene_Particles &particles = item.get<Scene_Particles>();
                                   unsigned int idx = (unsigned int) materials.size();
                                   materials.push_back(BSDF(BSDF_Lambertian(particles.opt.color.to_linear())));

                                   bool use_bvh = scene_use_bvh;
                                   futures.push_back(thread_pool.enqueue([&particles, use_bvh, idx]()
                                                                         {
                                                                             Tri_Mesh mesh(particles.mesh(), use_bvh);

                                                                             const auto &parts = particles.get_particles();
                                                                             std::vector<Object> particle_objs;

                                                                             for (const Scene_Particles::Particle &p: parts)
                                                                             {
                                                                                 Tri_Mesh copy = mesh.copy();
                                                                                 Mat4 T = Mat4::translate(p.pos) * Mat4::scale(Vec3{particles.opt.scale});
                                                                                 particle_objs.emplace_back(std::move(copy), particles.id(), idx, T);
                                                                             }

                                                                             return particle_objs;
                                                                         }));
                               }
                           });

    std::vector<Object> obj_list;

    for (auto &f: futures)
    {
        std::vector<Object> result = f.get();
        obj_list.reserve(obj_list.size() + result.size());
        std::move(std::begin(result), std::end(result), std::back_inserter(obj_list));
    }

    area_lights = List(std::move(area_light_list));
    build_lights(layout_scene);

    if (scene_use_bvh)
    {
        BVH<Object> scene_bvh(std::move(obj_list));
        scene = Object(std::move(scene_bvh));
    } else
    {
        List<Object> scene_list(std::move(obj_list));
        scene = Object(std::move(scene_list));
    }
}

void Pathtracer::set_samples(size_t samples)
{
    n_samples = samples;
}

void Pathtracer::set_params(size_t w, size_t h, size_t samples, size_t depth, bool use_bvh)
{
    out_w = w;
    out_h = h;
    n_samples = samples;
    max_depth = depth;
    scene_use_bvh = use_bvh;
    accumulator.resize(out_w, out_h);
}

void Pathtracer::log_ray(const Ray &ray, float t, Spectrum color)
{
    gui.log_ray(ray, t, color);
}

void Pathtracer::accumulate(const HDR_Image &sample)
{

    std::lock_guard<std::mutex> lock(accumulator_mut);

    accumulator_samples++;
    for (size_t j = 0; j < out_h; j++)
    {
        for (size_t i = 0; i < out_w; i++)
        {
            Spectrum &s = accumulator.at(i, j);
            const Spectrum &n = sample.at(i, j);
            s += (n - s) * (1.0f / accumulator_samples);
        }
    }
}

void Pathtracer::do_trace(size_t samples)
{

    HDR_Image sample(out_w, out_h);
    for (size_t j = 0; j < out_h; j++)
    {
        for (size_t i = 0; i < out_w; i++)
        {

            size_t sampled = 0;
            for (size_t s = 0; s < samples; s++)
            {

                Spectrum p = trace_pixel(i, j);
                if (p.valid())
                {
                    sample.at(i, j) += p;
                    sampled++;
                }

                if (cancel_flag)
                    return;
            }

            if (sampled > 0)
                sample.at(i, j) *= (1.0f / sampled);
        }
    }
    accumulate(sample);
}

bool Pathtracer::in_progress() const
{
    return completed_epochs.load() < total_epochs;
}

std::pair<float, float> Pathtracer::completion_time() const
{
    double freq = (double) SDL_GetPerformanceFrequency();
    return {(float) (build_time / freq), (float) (render_time / freq)};
}

float Pathtracer::progress() const
{
    return (float) completed_epochs.load() / (float) total_epochs;
}

size_t Pathtracer::visualize_bvh(GL::Lines &lines, GL::Lines &active, size_t depth)
{
    return scene.visualize(lines, active, depth, Mat4::I);
}

void Pathtracer::begin_render(Scene &layout_scene, const Camera &cam, bool add_samples)
{

    size_t n_threads = std::thread::hardware_concurrency();
    size_t samples_per_epoch = std::max(size_t(1), n_samples / (n_threads * 10));

    cancel();
    total_epochs = n_samples / samples_per_epoch + !!(n_samples % samples_per_epoch);

    if (!add_samples)
    {
        accumulator.clear({});
        accumulator_samples = 0;
        build_time = SDL_GetPerformanceCounter();
        build_scene(layout_scene);
        build_time = SDL_GetPerformanceCounter() - build_time;
    }
    render_time = SDL_GetPerformanceCounter();

    camera = cam;

    for (size_t s = 0; s < n_samples; s += samples_per_epoch)
    {
        size_t samples = (s + samples_per_epoch) > n_samples ? n_samples - s : samples_per_epoch;
        thread_pool.enqueue([samples, this]()
                            {
                                do_trace(samples);
                                size_t completed = completed_epochs++;
                                if (completed + 1 == total_epochs)
                                {
                                    Uint64 done = SDL_GetPerformanceCounter();
                                    render_time = done - render_time;
                                }
                            });
    }
}

void Pathtracer::cancel()
{
    cancel_flag = true;
    thread_pool.clear();
    completed_epochs = 0;
    total_epochs = 0;
    cancel_flag = false;
    if (completed_epochs < total_epochs)
        render_time = SDL_GetPerformanceCounter() - render_time;
}

const HDR_Image &Pathtracer::get_output()
{
    return accumulator;
}

const GL::Tex2D &Pathtracer::get_output_texture(float exposure)
{
    std::lock_guard<std::mutex> lock(accumulator_mut);
    return accumulator.get_texture(exposure);
}

Vec3 Pathtracer::sample_area_lights(Vec3 from)
{
    if (!area_lights.empty() && env_light.has_value())
    {
        if (RNG::coin_flip(0.5f))
            return env_light.value().sample();
        return area_lights.sample(from);
    }
    if (env_light.has_value())
    {
        return env_light.value().sample();
    }
    return area_lights.sample(from);
}

float Pathtracer::area_lights_pdf(Vec3 from, Vec3 dir)
{
    int n = 0;
    float pdf = 0.0f;
    if (!area_lights.empty())
    {
        pdf += area_lights.pdf(Ray(from, dir));
        n++;
    }
    if (env_light.has_value())
    {
        pdf += env_light.value().pdf(dir);
        n++;
    }
    if (n)
        pdf /= n;
    return pdf;
}

Spectrum Pathtracer::point_lighting(const Shading_Info &hit)
{

    if (hit.bsdf.is_discrete())
        return {};

    Spectrum radiance;
    for (auto &light: point_lights)
    {
        Light_Sample sample = light.sample(hit.pos);
        Vec3 in_dir = hit.world_to_object.rotate(sample.direction);

        Spectrum attenuation = hit.bsdf.evaluate(hit.out_dir, in_dir);
        if (attenuation.luma() == 0.0f)
            continue;

        Ray shadow_ray(hit.pos, sample.direction, Vec2{EPS_F, sample.distance - EPS_F});

        Trace shadow = scene.hit(shadow_ray);
        if (!shadow.hit)
        {
            radiance += attenuation * sample.radiance;
        }
    }

    return radiance;
}

} // namespace PT
