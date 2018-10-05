#include "debug_shapes.hpp"

namespace sushi {

void DebugLineShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
    vertices.push_back(start);
    vertices.push_back(end);

    colors.push_back(to_vec3(color, NormalizedColor{}));
    colors.push_back(to_vec3(color, NormalizedColor{}));
}

void DebugSphereShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
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

        vertices.push_back((center + a) * quat);
        vertices.push_back((center + b) * quat);

        for(int i = 0; i < 2; ++i) {
            colors.push_back(to_vec3(color, NormalizedColor{}));
        }
    }
}

void DebugAABBShape::construct(std::vector<Vec3>& vertices, std::vector<Vec3>& colors) const {
    const Vec3 min = center - extend;
    const Vec3 max = center + extend;

    // Top face
    vertices.emplace_back(min.x, max.y, min.z);
    vertices.emplace_back(max.x, max.y, min.z);

    vertices.emplace_back(max.x, max.y, min.z);
    vertices.emplace_back(max.x, max.y, max.z);

    vertices.emplace_back(max.x, max.y, max.z);
    vertices.emplace_back(min.x, max.y, max.z);

    vertices.emplace_back(min.x, max.y, max.z);
    vertices.emplace_back(min.x, max.y, min.z);

    // Bottom face
    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, min.z);

    vertices.emplace_back(max.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, max.z);

    vertices.emplace_back(max.x, min.y, max.z);
    vertices.emplace_back(min.x, min.y, max.z);

    vertices.emplace_back(min.x, min.y, max.z);
    vertices.emplace_back(min.x, min.y, min.z);

    // Front face
    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, min.z);

    vertices.emplace_back(max.x, min.y, min.z);
    vertices.emplace_back(max.x, max.y, min.z);

    vertices.emplace_back(max.x, max.y, min.z);
    vertices.emplace_back(min.x, max.y, min.z);

    vertices.emplace_back(min.x, max.y, min.z);
    vertices.emplace_back(min.x, min.y, min.z);

    // Back face
    vertices.emplace_back(min.x, min.y, max.z);
    vertices.emplace_back(max.x, min.y, max.z);

    vertices.emplace_back(max.x, min.y, max.z);
    vertices.emplace_back(max.x, max.y, max.z);

    vertices.emplace_back(max.x, max.y, max.z);
    vertices.emplace_back(min.x, max.y, max.z);

    vertices.emplace_back(min.x, max.y, max.z);
    vertices.emplace_back(min.x, min.y, max.z);

    // Left face
    vertices.emplace_back(min.x, min.y, min.z);
    vertices.emplace_back(min.x, min.y, max.z);

    vertices.emplace_back(min.x, min.y, max.z);
    vertices.emplace_back(min.x, max.y, max.z);

    vertices.emplace_back(min.x, max.y, max.z);
    vertices.emplace_back(min.x, max.y, min.z);

    vertices.emplace_back(min.x, max.y, min.z);
    vertices.emplace_back(min.x, min.y, min.z);

    // Right face
    vertices.emplace_back(max.x, min.y, min.z);
    vertices.emplace_back(max.x, min.y, max.z);

    vertices.emplace_back(max.x, min.y, max.z);
    vertices.emplace_back(max.x, max.y, max.z);

    vertices.emplace_back(max.x, max.y, max.z);
    vertices.emplace_back(max.x, max.y, min.z);

    vertices.emplace_back(max.x, max.y, min.z);
    vertices.emplace_back(max.x, min.y, min.z);

    static const int NB_FACES_PER_CUBE = 6;
    static const int NB_VERTICES_PER_FACE = 8;
    for(int i = 0 ; i < NB_VERTICES_PER_FACE * NB_FACES_PER_CUBE; ++i) {
        colors.push_back(to_vec3(color, NormalizedColor{}));
    }

}

}