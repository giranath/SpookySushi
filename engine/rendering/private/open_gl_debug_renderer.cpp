#include "open_gl_debug_renderer.hpp"

#include <job_service.hpp>
#include <log_service.hpp>
#include <engine.hpp>

#include <iterator>
#include <algorithm>

namespace sushi {

static const std::size_t MAX_DEBUG_LINE_COUNT = 10'000;
static const std::size_t MAX_DEBUG_AABBS_COUNT = 10'000;
static const std::size_t MAX_DEBUG_CIRCLES_COUNT = 1'000;
static const std::size_t MAX_DEBUG_SPHERES_COUNT = 500;

static const std::size_t SPHERE_RESOLUTION = 5;
static const std::size_t CIRCLE_RESOLUTION = 16;

void DebugLineShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
    if(cooldown == 0) return;

    vertices.push_back(start);
    vertices.push_back(end);

    colors.push_back(to_vec3(color, NormalizedColor{}));
    colors.push_back(to_vec3(color, NormalizedColor{}));
}

void DebugSphereShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
    if(cooldown == 0) return;

    const float HALF_CIRCLE_THETA = glm::pi<float>();

    for(int ring_index = 0; ring_index < SPHERE_RESOLUTION; ++ring_index) {
        for(int latitude_index = 0; latitude_index < SPHERE_RESOLUTION; ++latitude_index) {

        }
    }
}

static Vec3 orthogonal(Vec3 v) {
    const float x = std::abs(v.x);
    const float y = std::abs(v.y);
    const float z = std::abs(v.z);

    const Vec3 other = x < y ? (x < z ? Vec3{1.f, 0.f, 0.f} : Vec3{0.f, 0.f, 1.f}) : (y < z ? Vec3{0.f, 1.f, 0.f} : Vec3{0.f, 0.f, 1.f});
    return glm::cross(v, other);
}

static Quaternion rotation_between_unit_vectors(Vec3 u, Vec3 v) {
    const Vec3 w = glm::cross(u, v);
    const Quaternion q(1.f + glm::dot(u, v), w.x, w.y, w.z);
    return glm::normalize(q);
}

void DebugCircleShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
    if(cooldown == 0) return;

    const float FULL_CIRCLE_THETA = glm::pi<float>() * 2.f;

    Quaternion quat = rotation_between_unit_vectors(normal, Vec3{0.f, 1.f, 0.f});

    for(int i = 0; i < CIRCLE_RESOLUTION; ++i) {
        const float THETA = (static_cast<float>(i) / CIRCLE_RESOLUTION) * FULL_CIRCLE_THETA;
        const float NEXT_THETA = (static_cast<float>(i + 1) / CIRCLE_RESOLUTION) * FULL_CIRCLE_THETA;

        const float x = std::cos(THETA) * radius;
        const float y = std::sin(THETA) * radius;

        const float next_x = std::cos(NEXT_THETA) * radius;
        const float next_y = std::sin(NEXT_THETA) * radius;

        const Vec3 a(x, 0, y);
        const Vec3 b(next_x, 0, next_y);

        vertices.push_back(center * quat);
        vertices.push_back((center + a) * quat);
        vertices.push_back((center + b) * quat);

        for(int i = 0; i < 3; ++i) {
            colors.push_back(to_vec3(color, NormalizedColor{}));
        }
    }
}

void DebugAABBShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
    const Vec3 min = center - extend;
    const Vec3 max = center + extend;

    if(cooldown == 0) return;

    // Top face
    vertices.emplace_back(min.x, max.y, min.z);
    vertices.emplace_back(max.x, max.y, max.z);
    vertices.emplace_back(max.x, max.y, min.z);

    vertices.emplace_back(min.x, max.y, min.z);
    vertices.emplace_back(min.x, max.y, max.z);
    vertices.emplace_back(max.x, max.y, max.z);

    // Bottom face
    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, max.z);

    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, max.z);
    vertices.emplace_back(min.x, min.y, max.z);

    // Front face
    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(max.x, max.y, min.z);
    vertices.emplace_back(max.x, min.y, min.z);

    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(min.x, max.y, min.z);
    vertices.emplace_back(max.x, max.y, min.z);

    // Left face
    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(min.x, min.y, max.z);
    vertices.emplace_back(min.x, max.y, max.z);

    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(min.x, max.y, max.z);
    vertices.emplace_back(min.x, max.y, min.z);

    // Right face
    vertices.emplace_back(max.x, min.y, min.z);
    vertices.emplace_back(max.x, max.y, max.z);
    vertices.emplace_back(max.x, min.y, max.z);

    vertices.emplace_back(max.x, min.y, min.z);
    vertices.emplace_back(max.x, max.y, min.z);
    vertices.emplace_back(max.x, max.y, max.z);

    // Back face
    vertices.emplace_back(min.x, min.y, max.z);
    vertices.emplace_back(max.x, min.y, max.z);
    vertices.emplace_back(max.x, max.y, max.z);

    vertices.emplace_back(min.x, min.y, max.z);
    vertices.emplace_back(max.x, max.y, max.z);
    vertices.emplace_back(min.x, max.y, max.z);

    for(int i = 0 ; i < 36; ++i) {
        colors.push_back(to_vec3(color, NormalizedColor{}));
    }
}

OpenGLDebugRenderer::OpenGLDebugRenderer(RendererInterface* parent)
: renderer_{parent} {

}

void OpenGLDebugRenderer::init() {

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

    {
        lines_vao = gl::vertex_array::make();
        lines_vertices = gl::buffer::make();
        lines_colors = gl::buffer::make();

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(lines_vertices));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_LINE_COUNT * 2, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(lines_colors));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_LINE_COUNT * 2, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(lines_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(lines_vertices));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(lines_colors));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
    }

    {
        aabb_vao = gl::vertex_array::make();
        aabbs_vertices = gl::buffer::make();
        aabbs_colors = gl::buffer::make();

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(aabbs_vertices));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_AABBS_COUNT * 6 * 6, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(aabbs_colors));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_AABBS_COUNT * 6 * 6, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(aabb_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(aabbs_vertices));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(aabbs_colors));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
    }

    {
        circles_vao = gl::vertex_array::make();
        circles_vertices = gl::buffer::make();
        circles_colors = gl::buffer::make();

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(circles_vertices));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_CIRCLES_COUNT * CIRCLE_RESOLUTION * 3, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(circles_colors));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_CIRCLES_COUNT * CIRCLE_RESOLUTION * 3, nullptr, GL_DYNAMIC_DRAW);

        gl::bind(circles_vao);
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(circles_vertices));
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(circles_colors));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, nullptr);
    }

    {
        spheres_vertices = gl::buffer::make();
        gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(spheres_vertices));
        glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_SPHERES_COUNT * SPHERE_RESOLUTION * SPHERE_RESOLUTION * 3, nullptr, GL_DYNAMIC_DRAW);
    }

}

void OpenGLDebugRenderer::add_line(const Vec3& start_point, const Vec3& end_point, RGBColor line_color, uint32_t duration_ms, bool enable_depth) {
    lines.emplace_back(start_point, end_point, line_color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_sphere(const Vec3& center, float radius, RGBColor line_color, uint32_t duration_ms, bool enable_depth) {
    spheres.emplace_back(center, radius, line_color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_circle(const Vec3& center, const Vec3& normal, float radius, RGBColor color, uint32_t duration_ms, bool enabled_depth) {
    circles.emplace_back(center, radius, glm::normalize(normal), color, duration_ms, enabled_depth);
}

void OpenGLDebugRenderer::add_cross(const Vec3& position, RGBColor color_x, RGBColor color_y, RGBColor color_z, uint32_t duration_ms, bool enable_depth) {
    add_line(position, position + Vec3{1.f, 0.f, 0.f}, color_x, duration_ms, enable_depth);
    add_line(position, position + Vec3{0.f, 1.f, 0.f}, color_y, duration_ms, enable_depth);
    add_line(position, position + Vec3{0.f, 0.f, 1.f}, color_z, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_aabb(const Vec3& center, const Vec3& extend, RGBColor color, uint32_t duration_ms, bool enable_depth) {
    aabbs.emplace_back(center, extend, color, duration_ms, enable_depth);
}

void OpenGLDebugRenderer::add_triangle(const Vec3& a, const Vec3& b, const Vec3& c, RGBColor color, uint32_t duration_ms, bool enable_depth) {
    throw std::logic_error{"unimplemented"};
}

template<typename InputIterator>
auto reduce_cooldown(InputIterator start, InputIterator end, uint32_t dt_ms) {
    return [start, end, dt_ms](async::job&) {
        std::for_each(start, end, [dt_ms](DebugShape& value) {
            if(dt_ms > value.cooldown) {
                value.cooldown = 0;
            }
            else {
                value.cooldown -= dt_ms;
            }
        });
    };
}

template<typename InputIterator>
auto sort_primitive_queue(InputIterator begin, InputIterator end) {
    return [begin, end](async::job&) {
        std::sort(begin, end, [](const DebugShape& a, const DebugShape& b) {
            return a.sort_key() < b.sort_key();
        });
    };
}

template<typename InputIterator>
void reduce_and_sort_primitives_queue(InputIterator start, InputIterator end, uint32_t dt_ms, async::job* root_job) {
    async::job* clear_primitives_job = JobsService::get().make_closure(async::worker::mode::background, reduce_cooldown(start, end, dt_ms), root_job);
    JobsService::get().make_closure(async::worker::mode::background, sort_primitive_queue(start, end), clear_primitives_job);
}

void OpenGLDebugRenderer::update(uint32_t dt_ms) {
    // Lines
    async::job clean_job_root;
    reduce_and_sort_primitives_queue(std::begin(lines), std::end(lines), dt_ms, &clean_job_root);
    reduce_and_sort_primitives_queue(std::begin(aabbs), std::end(aabbs), dt_ms, &clean_job_root);
    reduce_and_sort_primitives_queue(std::begin(circles), std::end(circles), dt_ms, &clean_job_root);
    reduce_and_sort_primitives_queue(std::begin(spheres), std::end(spheres), dt_ms, &clean_job_root);

    JobsService::get().foreground()->wait_for(&clean_job_root);


}

void OpenGLDebugRenderer::draw() {
    assert(renderer());

    const Camera* active_camera = renderer()->active_camera();
    if(!active_camera) {
        log_warning("sushi.renderer.debug", "no active camera");
        return;
    }

    // Show all debug enabled primitives
    std::vector<Vec3> lines_raw_data;
    std::vector<Vec3> lines_raw_colors;
    std::for_each(std::begin(lines), std::end(lines), [&lines_raw_data, &lines_raw_colors](const DebugLineShape& shape) {
       shape.construct(lines_raw_data, lines_raw_colors);
    });

    gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(lines_vertices));
    glBufferSubData(GL_ARRAY_BUFFER, 0, lines_raw_data.size() * sizeof(Vec3), lines_raw_data.data());

    gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(lines_colors));
    glBufferSubData(GL_ARRAY_BUFFER, 0, lines_raw_data.size() * sizeof(Vec3), lines_raw_colors.data());

    std::vector<Vec3> aabb_raw_data;
    std::vector<Vec3> aabb_raw_colors;
    aabb_raw_data.reserve(aabbs.size() * 36); // 36 vertices per aabb
    std::for_each(std::begin(aabbs), std::end(aabbs), [&aabb_raw_data, &aabb_raw_colors](const DebugAABBShape& aabb) {
        aabb.construct(aabb_raw_data, aabb_raw_colors);
    });

    gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(aabbs_vertices));
    glBufferSubData(GL_ARRAY_BUFFER, 0, aabb_raw_data.size() * sizeof(Vec3), aabb_raw_data.data());

    gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(aabbs_colors));
    glBufferSubData(GL_ARRAY_BUFFER, 0, aabb_raw_colors.size() * sizeof(Vec3), aabb_raw_colors.data());

    std::vector<Vec3> circles_raw_data;
    std::vector<Vec3> circles_raw_colors;
    std::for_each(std::begin(circles), std::end(circles), [&circles_raw_data, &circles_raw_colors](const DebugCircleShape& shape) {
        shape.construct(circles_raw_data, circles_raw_colors);
    });

    gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(circles_vertices));
    glBufferSubData(GL_ARRAY_BUFFER, 0, circles_raw_data.size() * sizeof(Vec3), circles_raw_data.data());

    gl::bind(gl::buffer_bind<GL_ARRAY_BUFFER>(circles_colors));
    glBufferSubData(GL_ARRAY_BUFFER, 0, circles_raw_colors.size() * sizeof(Vec3), circles_raw_colors.data());

    // Enable wireframe
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);

    gl::bind(debug_program);
    projection_view_model_matrix_uniform.set(active_camera->projection() * active_camera->view());

    gl::bind(lines_vao);
    glDrawArrays(GL_LINES, 0, lines_raw_data.size() * 2);

    gl::bind(aabb_vao);
    glDrawArrays(GL_TRIANGLES, 0, aabb_raw_data.size() * 3);

    gl::bind(circles_vao);
    glDrawArrays(GL_TRIANGLES, 0, circles_raw_data.size() * 3);

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

}
