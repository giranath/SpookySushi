#include "open_gl_debug_renderer.hpp"

#include <job_service.hpp>
#include <log_service.hpp>
#include <engine.hpp>
#include <profiler.hpp>

#include <iterator>
#include <algorithm>

namespace sushi {

static const std::size_t MAX_DEBUG_LINE_COUNT = 50'000;
static const std::size_t MAX_DEBUG_AABBS_COUNT = 50'000;
static const std::size_t MAX_DEBUG_CIRCLES_COUNT = 50'000;
static const std::size_t MAX_DEBUG_SPHERES_COUNT = 50'000;
static const std::size_t MAX_DEBUG_TRIANGLES_COUNT = 25'000;

OpenGLDebugRenderer::OpenGLDebugRenderer(RendererInterface* parent)
: renderer_{parent}
, aabb_renderer{MAX_DEBUG_AABBS_COUNT}
, line_renderer{MAX_DEBUG_LINE_COUNT}
, circle_renderer{MAX_DEBUG_CIRCLES_COUNT}
, sphere_renderer{MAX_DEBUG_SPHERES_COUNT}
, triangle_renderer{MAX_DEBUG_TRIANGLES_COUNT} {

}

void OpenGLDebugRenderer::compile_shaders() {
    gl::vertex_shader vertex_shader;
    auto vertex_status = vertex_shader.compile(
            "#version 330\n"
            "\n"
            "layout(location = 0) in vec3 local_vert_position;\n"
            "layout(location = 1) in vec3 local_vert_color;\n"
            "\n"
            "uniform mat4 projection_view_model_matrix;\n"
            "\n"
            "out vec3 frag_color;\n"
            "\n"
            "void main() {\n"
            "    gl_Position = projection_view_model_matrix * vec4(local_vert_position, 1.0);\n"
            "\n"
            "    frag_color = local_vert_color;\n"
            "}");

    gl::fragment_shader fragment_shader;
    auto fragment_status = fragment_shader.compile(
            "#version 330\n"
            "\n"
            "out vec3 color;\n"
            "in vec3 frag_color;\n"
            "\n"
            "void main() {\n"
            "    color = frag_color;\n"
            "}");

    if(!fragment_status) {
        log_critical("sushi.renderering.debug", "failed to compile fragment shader: %s", fragment_status.message().c_str());
    }

    if(!vertex_status) {
        log_critical("sushi.renderering.debug", "failed to compile vertex shader: %s", vertex_status.message().c_str());
    }

    if(!fragment_status || !vertex_status) {
        throw std::runtime_error("failed to compile glsl shader");
    }

    debug_program = gl::program::make();
    debug_program.attach(vertex_shader);
    debug_program.attach(fragment_shader);
    gl::program::link_status status = debug_program.link();
    if(!status) {
        log_critical("sushi.renderering.debug", "failed to link shader program: %s", status.message().c_str());
        throw std::runtime_error("failed to link glsl program");
    }

    projection_view_model_matrix_uniform = debug_program.find_uniform<Mat4x4>("projection_view_model_matrix");
    if(projection_view_model_matrix_uniform) {
        log_critical("sushi.rendering.debug", "failed to retrieve uniform 'projection_view_model_matrix'");
        throw std::runtime_error("failed to retrieve uniform");
    }
}

void OpenGLDebugRenderer::init() {
    compile_shaders();

    aabb_renderer.init();
    line_renderer.init();
    circle_renderer.init();
    sphere_renderer.init();
    triangle_renderer.init();
}

void OpenGLDebugRenderer::add_line(const Vec3& start_point, const Vec3& end_point, RGBColor line_color, uint32_t duration_ms, bool enable_depth) {
    //assert(!line_renderer.full());
    line_renderer.emplace(start_point, end_point, line_color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_sphere(const Vec3& center, float radius, RGBColor line_color, uint32_t duration_ms, bool enable_depth) {
    //assert(!sphere_renderer.full());
    sphere_renderer.emplace(center, radius, line_color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_circle(const Vec3& center, const Vec3& normal, float radius, RGBColor color, uint32_t duration_ms, bool enabled_depth) {
    //assert(!circle_renderer.full());
    circle_renderer.emplace(center, radius, glm::normalize(normal), color, duration_ms, enabled_depth);
}

void OpenGLDebugRenderer::add_cross(const Vec3& position, RGBColor color_x, RGBColor color_y, RGBColor color_z, uint32_t duration_ms, bool enable_depth) {
    add_line(position, position + Vec3{1.f, 0.f, 0.f}, color_x, duration_ms, enable_depth);
    add_line(position, position + Vec3{0.f, 1.f, 0.f}, color_y, duration_ms, enable_depth);
    add_line(position, position + Vec3{0.f, 0.f, 1.f}, color_z, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_aabb(const Vec3& center, const Vec3& extend, RGBColor color, uint32_t duration_ms, bool enable_depth) {
    //assert(!aabb_renderer.full());
    aabb_renderer.emplace(center, extend, color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_triangle(const Vec3& a, const Vec3& b, const Vec3& c, RGBColor color, uint32_t duration_ms, bool enable_depth) {
    //assert(!triangle_renderer.full());
    triangle_renderer.emplace(a, b, c, color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::update(uint32_t dt_ms) {
    //async::job clean_job_root;
    //debug::ScopedProfile update_profile(69);

    //JobsService::get().make_closure(async::worker::mode::background, [this, dt_ms](async::job&) {
    //    debug::ScopedProfile profile(70);
        aabb_renderer.update(dt_ms);
    //}, &clean_job_root);

    //JobsService::get().make_closure(async::worker::mode::background, [this, dt_ms](async::job&) {
    //    debug::ScopedProfile profile(71);
        line_renderer.update(dt_ms);
    //}, &clean_job_root);

    //JobsService::get().make_closure(async::worker::mode::background, [this, dt_ms](async::job&) {
    //    debug::ScopedProfile profile(72);
        circle_renderer.update(dt_ms);
    //}, &clean_job_root);

    //JobsService::get().make_closure(async::worker::mode::background, [this, dt_ms](async::job&) {
    //    debug::ScopedProfile profile(73);
        sphere_renderer.update(dt_ms);
    //}, &clean_job_root);

    //JobsService::get().make_closure(async::worker::mode::background, [this, dt_ms](async::job&) {
    //    debug::ScopedProfile profile(74);
        triangle_renderer.update(dt_ms);
    //}, &clean_job_root);

    //JobsService::get().foreground()->wait_for(&clean_job_root);
}

void OpenGLDebugRenderer::collect_garbages() {
    //async::job collect_root_job;

    //JobsService::get().make_closure(async::worker::mode::background, [this](async::job&) {
        aabb_renderer.collect_garbages();
    //}, &collect_root_job);

    //JobsService::get().make_closure(async::worker::mode::background, [this](async::job&) {
        line_renderer.collect_garbages();
    //}, &collect_root_job);

    //JobsService::get().make_closure(async::worker::mode::background, [this](async::job&) {
        circle_renderer.collect_garbages();
    //}, &collect_root_job);

    //JobsService::get().make_closure(async::worker::mode::background, [this](async::job&) {
        sphere_renderer.collect_garbages();
    //}, &collect_root_job);

    //JobsService::get().make_closure(async::worker::mode::background, [this](async::job&) {
        triangle_renderer.collect_garbages();
    //}, &collect_root_job);

    //JobsService::get().foreground()->wait_for(&collect_root_job);
}

void OpenGLDebugRenderer::draw() {
    assert(renderer());

    const Camera* active_camera = renderer()->active_camera();
    if(!active_camera) {
        log_warning("sushi.renderer.debug", "no active camera");
        return;
    }

    // Batch all the shapes
    aabb_renderer.batch_shapes();
    line_renderer.batch_shapes();
    circle_renderer.batch_shapes();
    sphere_renderer.batch_shapes();
    triangle_renderer.batch_shapes();

    // Enable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);

    // Setup shader
    gl::bind(debug_program);
    projection_view_model_matrix_uniform.set(active_camera->projection() * active_camera->view());

    // Render all the shapes
    aabb_renderer.render();
    line_renderer.render();
    circle_renderer.render();
    sphere_renderer.render();
    triangle_renderer.render();

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    collect_garbages();
}

}
