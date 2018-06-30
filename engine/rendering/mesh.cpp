#include "mesh.hpp"

#include <cassert>

namespace sushi {

Mesh::Mesh(const std::vector<Vec3>& vertices)
: vertices_(vertices) {

}

Mesh::Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs)
: vertices_(vertices)
, uvs_(uvs) {
    if(vertices.size() != uvs.size()) {
        throw InvalidMeshError{};
    }
}

Mesh::Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec3>& normals)
: vertices_(vertices)
, normals_(normals) {
    if(vertices.size() != normals.size()) {
        throw InvalidMeshError{};
    }
}

Mesh::Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs, const std::vector<Vec3>& normals)
: vertices_(vertices)
, normals_(normals)
, uvs_(uvs) {
    if(vertices.size() != uvs.size() && vertices.size() != normals.size()) {
        throw InvalidMeshError{};
    }
}

bool Mesh::use_uvs() const noexcept {
    return !uvs_.empty();
}

bool Mesh::use_normals() const noexcept {
    return !normals_.empty();
}

std::size_t Mesh::size() const noexcept {
    return vertices_.size();
}

const std::vector<Vec3>& Mesh::vertices() const noexcept {
    return vertices_;
}

const std::vector<Vec2>& Mesh::uvs() const noexcept {
    return uvs_;
}

const std::vector<Vec3>& Mesh::normals() const noexcept {
    return normals_;
}

}