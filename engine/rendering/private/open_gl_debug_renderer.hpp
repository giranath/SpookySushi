#ifndef SPOOKYSUSHI_OPEN_GL_DEBUG_RENDERER_HPP
#define SPOOKYSUSHI_OPEN_GL_DEBUG_RENDERER_HPP

#include <renderer_interface.hpp>

namespace sushi {

class OpenGLDebugRenderer : public DebugRendererInterface {
public:
    void add_line(const Vec3& start_point, const Vec3& end_point, RGBColor line_color, float line_width, uint32_t duration_ms, bool enable_depth) override;
    void add_sphere(const Vec3& center, float radius, RGBColor line_color, uint32_t duration_ms, bool enable_depth) override;
    void add_circle(const Vec3& center, const Vec3& normal, float radius, RGBColor color, uint32_t duration_ms, bool enabled_depth) override;
    void add_cross(const Vec3& position, RGBColor color, uint32_t duration_ms, bool enable_depth) override;
    void add_aabb(const Vec3& center, const Vec3& extend, RGBColor color, uint32_t duration_ms, bool enable_depth) override;
    void add_triangle(const Vec3& a, const Vec3& b, const Vec3& c, RGBColor color, uint32_t duration_ms, bool enable_depth) override;
    void draw() override;
};

}

#endif
