#include "../geometry/util.h"
#include "../scene/renderer.h"

#include "manager.h"
#include "simulate.h"

namespace Gui
{

const char *Solid_Type_Names[(int) Solid_Type::count] = {"Sphere", "Cube", "Cylinder", "Torus", "Custom"};

Simulate::Simulate() : thread_pool(std::thread::hardware_concurrency())
{
    last_update = SDL_GetPerformanceCounter();
}

Simulate::~Simulate()
{
    thread_pool.wait();
    thread_pool.stop();
}

auto Simulate::keydown(Widgets &widgets, Undo &undo, SDL_Keysym key) -> bool
{
    return false;
}

void Simulate::step(Scene &scene, float dt)
{
    if (HinaPE::PhysicsSystem::instance().running)
        for (int i = 0; i < HinaPE::PhysicsSystem::instance().sub_step; ++i)
            HinaPE::PhysicsSystem::instance()._tick_(dt / (float) 5); // TODO: move this to separate physics thread;
    scene.for_items([this, dt](Scene_Item &item) { item.step(scene_obj, dt); });
}

void Simulate::update_time()
{
    last_update = SDL_GetPerformanceCounter();
}

void Simulate::update(Scene &scene, Undo &undo)
{

    update_bvh(scene, undo);

    static Uint64 ufreq = SDL_GetPerformanceFrequency();
    static double freq = (double) ufreq;

    Uint64 time = SDL_GetPerformanceCounter();
    Uint64 udt = time - last_update;

    float dt = clamp((float) (udt / freq), 0.0f, 0.05f);
    last_update = time;

    step(scene, dt);
}

void Simulate::render(Scene_Maybe obj_opt, Widgets &widgets, Camera &cam)
{

    if (!obj_opt.has_value())
        return;
    Scene_Item &item = obj_opt.value();

    if (item.is<Scene_Light>())
    {
        Scene_Light &light = item.get<Scene_Light>();
        if (light.is_env())
            return;
    }

    Mat4 view = cam.get_view();
    item.render(view);
    Renderer::get().outline(view, item);

    Pose &pose = item.pose();
    float scale = std::min((cam.pos() - pose.pos).norm() / 5.5f, 10.0f);
    widgets.render(view, pose.pos, scale);
}

void Simulate::build_scene(Scene &scene)
{

    if (!scene.has_sim())
        return;

    std::vector<PT::Object> obj_list;
    std::vector<std::future<PT::Object>> futures;

    scene.for_items([&, this](Scene_Item &item)
                    {
                        if (item.is<Scene_Object>())
                        {
                            Scene_Object &obj = item.get<Scene_Object>();
                            futures.push_back(thread_pool.enqueue([&]()
                                                                  {
                                                                      if (obj.is_shape())
                                                                      {
                                                                          PT::Shape shape(obj.opt.shape);
                                                                          return PT::Object(std::move(shape), obj.id(), 0, obj.pose.transform());
                                                                      } else
                                                                      {
                                                                          PT::Tri_Mesh mesh(obj.posed_mesh(), use_bvh);
                                                                          return PT::Object(std::move(mesh), obj.id(), 0, obj.pose.transform());
                                                                      }
                                                                  }));
                        }
                    });

    for (auto &f: futures)
    {
        obj_list.push_back(f.get());
    }

    if (use_bvh)
    {
        scene_obj = PT::Object(PT::BVH<PT::Object>(std::move(obj_list)));
    } else
    {
        scene_obj = PT::Object(PT::List<PT::Object>(std::move(obj_list)));
    }
}

void Simulate::clear_particles(Scene &scene)
{
    scene.for_items([](Scene_Item &item)
                    {
                        if (item.is<Scene_Particles>())
                        {
                            item.get<Scene_Particles>().clear();
                        }
                    });
}

void Simulate::update_bvh(Scene &scene, Undo &undo)
{
    if (cur_actions != undo.n_actions())
    {
        build_scene(scene);
        cur_actions = undo.n_actions();
    }
}

auto Simulate::UIsidebar(Manager &manager, Scene &scene, Undo &undo, Widgets &widgets, Scene_Maybe obj_opt) -> Mode
{
    unsigned int idx = 0;

    Mode mode = Mode::simulate;
    if (obj_opt.has_value())
    {
        ImGui::Text("Object Options");
        mode = manager.item_options(undo, mode, obj_opt.value(), old_pose);
        ImGui::Separator();
    }

    update_bvh(scene, undo);

    ImGui::Text("Simulation");

    if (ImGui::Checkbox("Use BVH", &use_bvh))
    {
        clear_particles(scene);
        build_scene(scene);
    }

    if (ImGui::CollapsingHeader("Add New Emitter"))
    {
        ImGui::PushID(idx++);

        static Scene_Particles::Options gui_opt;
        static Solid_Type gui_type;
        ImGui::ColorEdit3("Color", gui_opt.color.data);
        ImGui::DragFloat("Speed", &gui_opt.velocity, 0.1f, 0.0f, std::numeric_limits<float>::max(), "%.2f");
        ImGui::SliderFloat("Angle", &gui_opt.angle, 0.0f, 180.0f, "%.2f");
        ImGui::DragFloat("Scale", &gui_opt.scale, 0.01f, 0.01f, 1.0f, "%.2f");
        ImGui::DragFloat("Lifetime", &gui_opt.lifetime, 0.01f, 0.0f, std::numeric_limits<float>::max(), "%.2f");
        ImGui::DragFloat("Particles/Sec", &gui_opt.pps, 1.0f, 1.0f, std::numeric_limits<float>::max(), "%.2f");
        ImGui::Checkbox("Enabled", &gui_opt.enabled);

        int n_types = (int) Solid_Type::count;
        if (!scene.has_obj())
        {
            n_types--;
            if (gui_type == Solid_Type::custom)
                gui_type = Solid_Type::sphere;
        }
        ImGui::Combo("Particle", (int *) &gui_type, Solid_Type_Names, n_types);

        std::vector<char *> names;
        std::vector<Scene_ID> ids;
        static int name_idx = 0;
        if (gui_type == Solid_Type::custom)
        {
            scene.for_items([&names, &ids](Scene_Item &item)
                            {
                                if (item.is<Scene_Object>() && item.get<Scene_Object>().is_editable())
                                {
                                    names.push_back(item.name().first);
                                    ids.push_back(item.id());
                                }
                            });
            ImGui::Combo("Mesh", &name_idx, names.data(), (int) names.size());
            name_idx = clamp(name_idx, 0, (int) names.size());
        }

        if (ImGui::Button("Add"))
        {
            GL::Mesh mesh;
            switch (gui_type)
            {
                case Solid_Type::cube:
                {
                    mesh = Util::cube_mesh(1.0f);
                }
                    break;
                case Solid_Type::cylinder:
                {
                    mesh = Util::cyl_mesh(0.5f, 1.0f, 8);
                }
                    break;
                case Solid_Type::torus:
                {
                    mesh = Util::torus_mesh(0.5f, 1.0f, 12, 8);
                }
                    break;
                case Solid_Type::sphere:
                {
                    mesh = Util::sphere_mesh(1.0f, 1);
                }
                    break;
                case Solid_Type::custom:
                {
                    mesh = scene.get<Scene_Object>(ids[name_idx]).mesh().copy();
                }
                    break;
                default:
                    break;
            }
            Scene_Particles particles(scene.reserve_id(), std::move(mesh));
            particles.opt.color = gui_opt.color;
            particles.opt.velocity = gui_opt.velocity;
            particles.opt.angle = gui_opt.angle;
            particles.opt.scale = gui_opt.scale;
            particles.opt.lifetime = gui_opt.lifetime;
            particles.opt.pps = gui_opt.pps;
            particles.opt.enabled = gui_opt.enabled;
            undo.add(std::move(particles));
        }

        ImGui::PopID();
    }

    if (ImGui::CollapsingHeader("Add New Fluid Bound"))
    {
        ImGui::PushID(idx++);
        static float R = 1.0f;
        ImGui::SliderFloat("Side Length", &R, 0.01f, 10.0f, "%.2f");
        if (ImGui::Button("Add"))
        {
            Halfedge_Mesh hm;
            hm.from_mesh(Util::cube_mesh(R / 2.0f));
            hm.flip(); // if flip
            undo.add_obj(std::move(hm), "Cube");
        }
        ImGui::PopID();
    }

    return mode;
}

} // namespace Gui
