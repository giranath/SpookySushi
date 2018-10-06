#ifndef SPOOKYSUSHI_RENDERER_INTERFACE_HPP
#define SPOOKYSUSHI_RENDERER_INTERFACE_HPP

#include "mesh.hpp"
#include "camera.hpp"
#include <types.hpp>
#include <color.hpp>

namespace sushi {

// The client pushes draw commands
// Such as: draw-mesh

class StaticMeshBuilder;

struct DebugRendererInterface;

struct RendererInterface {
    virtual ~RendererInterface() = default;

    /// Check if the renderer is ready
    virtual bool ready() const = 0;

    virtual bool initialize() = 0;
    virtual void uninitialize() = 0;

    virtual void start_frame_rendering() = 0;
    virtual void stop_frame_rendering() = 0;

    virtual StaticMeshBuilder& static_mesh_builder() const = 0;
    virtual DebugRendererInterface& debug_renderer() const = 0;

    virtual Camera* active_camera() = 0;
    virtual const Camera* active_camera() const = 0;
    virtual void set_active_camera(Camera* camera) = 0;
};

struct DebugRendererInterface {
    virtual ~DebugRendererInterface() = default;

    virtual void add_line(const Vec3& start_point, const Vec3& end_point, RGBColor line_color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_sphere(const Vec3& center, float radius, RGBColor line_color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_circle(const Vec3& center, const Vec3& normal, float radius, RGBColor color, uint32_t duration_ms = 0, bool enabled_depth = true) = 0;
    virtual void add_cross(const Vec3& position, RGBColor color_x = Colors::Red, RGBColor color_y = Colors::Green, RGBColor color_z = Colors::Blue, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    void add_cross(const Vec3& position, uint32_t duration_ms = 0, bool enable_depth = true) {
        add_cross(position, Colors::Red, Colors::Green, Colors::Blue, duration_ms, enable_depth);
    }
    virtual void add_aabb(const Vec3& center, const Vec3& extend, RGBColor color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_triangle(const Vec3& a, const Vec3& b, const Vec3& c, RGBColor color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;

    virtual void init() = 0;
    virtual void update(uint32_t dt_ms) = 0;
    virtual void draw() = 0;

    virtual RendererInterface* renderer() = 0;
    virtual const RendererInterface* renderer() const = 0;
};

}

#endif
