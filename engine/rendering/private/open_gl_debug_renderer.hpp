#ifndef SPOOKYSUSHI_OPEN_GL_DEBUG_RENDERER_HPP
#define SPOOKYSUSHI_OPEN_GL_DEBUG_RENDERER_HPP

#include "opengl_debug_shape_renderer.hpp"

#include <renderer_interface.hpp>
#include <vector>
#include <buffer.hpp>
#include <program.hpp>
#include <vertex_array.hpp>
#include <algorithm>
#include <iterator>

namespace sushi {

class OpenGLDebugRenderer : public DebugRendererInterface {
    OpenGLDebugShapeRenderer<DebugAABBShape> aabb_renderer;
    OpenGLDebugShapeRenderer<DebugLineShape> line_renderer;
    OpenGLDebugShapeRenderer<DebugCircleShape> circle_renderer;
    OpenGLDebugShapeRenderer<DebugSphereShape> sphere_renderer;
    OpenGLDebugShapeRenderer<DebugTriangleShape> triangle_renderer;

    gl::program debug_program;

    RendererInterface* renderer_;

    gl::uniform<Mat4x4> projection_view_model_matrix_uniform;

    void collect_garbages();

    void compile_shaders();

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
