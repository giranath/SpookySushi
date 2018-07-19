#include "mesh_builder.hpp"

namespace sushi {

void MeshBuilder::add_vertex(const Vec3& position) {
    assert(normals.empty());
    assert(uvs.empty());

    positions.push_back(position);
}

void MeshBuilder::add_vertex(const Vec3& position, const Vec2& uv) {
    assert(positions.size() == uvs.size());
    assert(normals.empty());

    positions.push_back(position);
    uvs.push_back(uv);
}

void MeshBuilder::add_vertex(const Vec3& position, const Vec3& normal) {
    assert(positions.size() == normals.size());
    assert(uvs.empty());

    positions.push_back(position);
    normals.push_back(normal);
}

void MeshBuilder::add_vertex(const Vec3& position, const Vec2& uv, const Vec3& normal) {
    assert(positions.size() == uvs.size());
    assert(positions.size() == normals.size());

    positions.push_back(position);
    uvs.push_back(uv);
    normals.push_back(normal);
}

void MeshBuilder::clear() noexcept {
    positions.clear();
    uvs.clear();
    normals.clear();
}

Mesh MeshBuilder::build() const noexcept {
    if(uvs.empty() && normals.empty()) {
        return Mesh{positions};
    }
    else if(uvs.empty()) {
        return Mesh{positions, normals};
    }
    else if(normals.empty()) {
        return Mesh{positions, uvs};
    }
    else {
        return Mesh{positions, uvs, normals};
    }
}

}