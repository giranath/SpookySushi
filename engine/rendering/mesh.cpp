#include "mesh.hpp"

#include <cassert>

namespace sushi {

void mesh::clear() noexcept {
    vertices.clear();
    uvs.clear();
    normals.clear();
}

bool mesh::use_uvs() const noexcept {
    return !uvs.empty();
}

bool mesh::use_normals() const noexcept {
    return !normals.empty();
}

std::size_t mesh::size() const noexcept {
    return vertices.size();
}

void mesh::push_vertex(glm::vec3 vertex) {
    assert(uvs.empty());
    assert(normals.empty());

    vertices.push_back(vertex);
}

void mesh::push_vertex(glm::vec3 vertex, glm::vec2 uv) {
    assert(vertices.size() == uvs.size());
    assert(normals.empty());

    vertices.push_back(vertex);
    uvs.push_back(uv);
}

void mesh::push_vertex(glm::vec3 vertex, glm::vec3 normal) {
    assert(vertices.size() == normals.size());
    assert(uvs.empty());

    vertices.push_back(vertex);
    normals.push_back(normal);
}

void mesh::push_vertex(glm::vec3 vertex, glm::vec2 uv, glm::vec3 normal) {
    assert(vertices.size() == uvs.size() == normals.size());

    vertices.push_back(vertex);
    uvs.push_back(uv);
    normals.push_back(normal);
}

}