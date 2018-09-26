#ifndef SPOOKYSUSHI_RENDERER_INTERFACE_HPP
#define SPOOKYSUSHI_RENDERER_INTERFACE_HPP

#include "mesh.hpp"
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
};

struct DebugRendererInterface {
    virtual ~DebugRendererInterface() = default;

    virtual void add_line(const Vec3& start_point, const Vec3& end_point, RGBColor line_color, float line_width = 1.0f, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_sphere(const Vec3& center, float radius, RGBColor line_color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_circle(const Vec3& center, const Vec3& normal, float radius, RGBColor color, uint32_t duration_ms = 0, bool enabled_depth = true) = 0;
    virtual void add_cross(const Vec3& position, RGBColor color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_aabb(const Vec3& center, const Vec3& extend, RGBColor color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;
    virtual void add_triangle(const Vec3& a, const Vec3& b, const Vec3& c, RGBColor color, uint32_t duration_ms = 0, bool enable_depth = true) = 0;

    virtual void draw() = 0;
};

}

#endif
