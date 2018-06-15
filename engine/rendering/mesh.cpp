#include "mesh.hpp"

#include <cassert>

namespace sushi {

void mesh::clear() noexcept {
    vertices_.clear();
    uvs_.clear();
    normals_.clear();
}

mesh::mesh(const std::vector<vec3>& vertices)
: vertices_(vertices) {

}

mesh::mesh(const std::vector<vec3>& vertices, const std::vector<vec2>& uvs)
: vertices_(vertices)
, uvs_(uvs) {
    if(vertices.size() != uvs.size()) {
        throw invalid_mesh_error{};
    }
}

mesh::mesh(const std::vector<vec3>& vertices, const std::vector<vec3>& normals)
: vertices_(vertices)
, normals_(normals) {
    if(vertices.size() != normals.size()) {
        throw invalid_mesh_error{};
    }
}

mesh::mesh(const std::vector<vec3>& vertices, const std::vector<vec2>& uvs, const std::vector<vec3>& normals)
: vertices_(vertices)
, normals_(normals)
, uvs_(uvs) {
    if(vertices.size() != uvs.size() && vertices.size() != normals.size()) {
        throw invalid_mesh_error{};
    }
}

bool mesh::use_uvs() const noexcept {
    return !uvs_.empty();
}

bool mesh::use_normals() const noexcept {
    return !normals_.empty();
}

std::size_t mesh::size() const noexcept {
    return vertices_.size();
}

const std::vector<vec3>& mesh::vertices() const noexcept {
    return vertices_;
}

const std::vector<vec2>& mesh::uvs() const noexcept {
    return uvs_;
}

const std::vector<vec3>& mesh::normals() const noexcept {
    return normals_;
}

}