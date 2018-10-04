#ifndef SPOOKYSUSHI_OPEN_GL_DEBUG_RENDERER_HPP
#define SPOOKYSUSHI_OPEN_GL_DEBUG_RENDERER_HPP

#include <renderer_interface.hpp>
#include <vector>
#include <buffer.hpp>
#include <program.hpp>
#include <vertex_array.hpp>

namespace sushi {

struct DebugShape {
    RGBColor color;
    uint32_t cooldown;
    bool enable_depth;

    constexpr DebugShape(RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : color{color}
    , cooldown{cooldown}
    , enable_depth{enable_depth} {

    }

    virtual void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const = 0;

    virtual uint32_t sort_key() const noexcept {
        const uint32_t DEPLETED_COOLDOWN_MASK = (1u << 31);
        const uint32_t DEPTH_ENABLED_MASK = (1u << 30);

        uint32_t sort_mask = 0;

        if(cooldown == 0) {
            sort_mask |= DEPLETED_COOLDOWN_MASK;
        }

        if(enable_depth) {
            sort_mask |= DEPTH_ENABLED_MASK;
        }

        return sort_mask;
    }
};

struct DebugCenteredShape : public DebugShape {
    Vec3 center;

    constexpr DebugCenteredShape(Vec3 center, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugShape(color, cooldown, enable_depth)
    , center{center} {

    }
};

struct DebugCircleShape : public DebugCenteredShape {
    Vec3 normal;
    float radius;

    constexpr DebugCircleShape(Vec3 center, float radius, Vec3 normal, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugCenteredShape(center, color, cooldown, enable_depth)
    , normal{normal}
    , radius{radius} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

struct DebugSphereShape : public DebugCenteredShape {
    float radius;

    constexpr DebugSphereShape(Vec3 center, float radius, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugCenteredShape(center, color, cooldown, enable_depth)
    , radius{radius} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

struct DebugAABBShape : public DebugCenteredShape {
    Vec3 extend;

    constexpr DebugAABBShape(Vec3 center, Vec3 extend, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugCenteredShape(center, color, cooldown, enable_depth)
    , extend{extend} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

struct DebugLineShape : public DebugShape {
    Vec3 start;
    Vec3 end;

    constexpr DebugLineShape(Vec3 start, Vec3 end, RGBColor color, uint32_t cooldown = 0, bool enable_depth = true) noexcept
    : DebugShape(color, cooldown, enable_depth)
    , start{start}
    , end{end} {

    }

    void construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const override;
};

class OpenGLDebugRenderer : public DebugRendererInterface {
    std::vector<DebugLineShape> lines;
    std::vector<DebugAABBShape> aabbs;
    std::vector<DebugCircleShape> circles;
    std::vector<DebugSphereShape> spheres;

    gl::buffer lines_vertices;
    gl::buffer lines_colors;
    gl::buffer aabbs_vertices;
    gl::buffer aabbs_colors;
    gl::buffer circles_vertices;
    gl::buffer circles_colors;
    gl::buffer spheres_vertices;
    gl::program debug_program;

    gl::vertex_array lines_vao;
    gl::vertex_array aabb_vao;
    gl::vertex_array circles_vao;
    RendererInterface* renderer_;

    gl::uniform<Mat4x4> projection_view_model_matrix_uniform;
public:
    OpenGLDebugRenderer(RendererInterface* parent);

    void add_line(const Vec3& start_point, const Vec3& end_point, RGBColor line_color, uint32_t duration_ms, bool enable_depth) override;
    void add_sphere(const Vec3& center, float radius, RGBColor line_color, uint32_t duration_ms, bool enable_depth) override;
    void add_circle(const Vec3& center, const Vec3& normal, float radius, RGBColor color, uint32_t duration_ms, bool enabled_depth) override;
    void add_cross(const Vec3& position, RGBColor color_x, RGBColor color_y, RGBColor color_z, uint32_t duration_ms, bool enable_depth) override;
    void add_aabb(const Vec3& center, const Vec3& extend, RGBColor color, uint32_t duration_ms, bool enable_depth) override;
    void add_triangle(const Vec3& a, const Vec3& b, const Vec3& c, RGBColor color, uint32_t duration_ms, bool enable_depth) override;

    void init() override;
    void update(uint32_t dt_ms) override;
    void draw() override;

    RendererInterface* renderer() override { return renderer_; }
    const RendererInterface* renderer() const override { return renderer_; }
};

}

#endif
