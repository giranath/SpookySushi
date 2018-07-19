#include "static_mesh_builder.hpp"

#include <cassert>

namespace sushi {

void StaticMeshDefinition::clear() {
    positions_.clear();
    uvs_.clear();
    normals_.clear();
}

void StaticMeshDefinition::add(const vertex::Position& position) {
    assert(uvs_.empty());
    assert(normals_.empty());

    positions_.push_back(position);
}

void StaticMeshDefinition::add(const vertex::Position& position, const vertex::UV& uv) {
    assert(normals_.empty());
    assert(positions_.size() == uvs_.size());

    positions_.push_back(position);
    uvs_.push_back(uv);
}

void StaticMeshDefinition::add(const vertex::Position& position, const vertex::Normal& normal) {
    assert(uvs_.empty());
    assert(positions_.size() == normals_.size());

    positions_.push_back(position);
    normals_.push_back(normal);
}

void StaticMeshDefinition::add(const vertex::Position& position, const vertex::UV& uv, const vertex::Normal& normal) {
    assert(positions_.size() == normals_.size());
    assert(positions_.size() == uvs_.size());

    positions_.push_back(position);
    normals_.push_back(normal);
    uvs_.push_back(uv);
}

bool StaticMeshDefinition::good() const noexcept {
    const bool has_normals = uses_normals();
    const bool has_uvs = uses_uvs();

    if(has_normals && has_uvs) {
        return positions_.size() == normals_.size() && positions_.size() == uvs_.size();
    }
    else if(has_normals) {
        return positions_.size() == normals_.size();
    }
    else if(has_uvs) {
        return positions_.size() == uvs_.size();
    }
    else {
        return true;
    }
}

bool StaticMeshDefinition::uses_normals() const noexcept {
    return !normals_.empty();
}

bool StaticMeshDefinition::uses_uvs() const noexcept {
    return !uvs_.empty();
}

const std::vector<vertex::Position>& StaticMeshDefinition::positions() const noexcept {
    return positions_;
}

}