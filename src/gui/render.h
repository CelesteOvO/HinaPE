#ifndef HINAPE_RENDER_H
#define HINAPE_RENDER_H

#include <SDL2/SDL.h>
#include <mutex>

#include "../platform/gl.h"
#include "../rays/pathtracer.h"
#include "../scene/scene.h"
#include "../util/camera.h"

#include "widgets.h"

struct Launch_Settings;

namespace Gui
{

enum class Mode;
class Manager;

class Render
{
public:
    Render(Scene &scene, Vec2 dim);

    std::string headless_render(Animate &animate, Scene &scene, const Launch_Settings &set);
    std::pair<float, float> completion_time() const;

    bool keydown(Widgets &widgets, SDL_Keysym key);
    Mode UIsidebar(Manager &manager, Undo &undo, Scene &scene, Scene_Maybe selected, Camera &user_cam);
    void render(Scene_Maybe obj, Widgets &widgets, Camera &user_cam);

    void update_dim(Vec2 dim);
    void load_cam(Vec3 pos, Vec3 front, float ar, float fov, float ap, float dist);
    const Camera &get_cam() const;

private:
    GL::Lines bvh_viz, bvh_active;
    Widget_Camera ui_camera;
    Widget_Render ui_render;
    Pose old_pose;

    // GUI Data
    bool render_ray_log = false;
    bool visualize_bvh = false;
    int bvh_level = 0;
    size_t bvh_levels = 0;
};

} // namespace Gui

#endif
