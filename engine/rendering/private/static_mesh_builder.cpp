#include "static_mesh_builder.hpp"

#include <cassert>
#include <algorithm>
#include <iterator>

namespace sushi {

StaticMeshDefinition::StaticMeshDefinition(const MeshAsset& asset)
: positions_{asset.vertices()}
, normals_{asset.normals()}
, uvs_{asset.texture_coords()}
, indices_{}{
    if(std::any_of(std::begin(asset.indices()), std::end(asset.indices()), [](const uint32_t index) { return index > std::numeric_limits<uint16_t>::max(); })) {
        throw std::out_of_range{"indices are out of range"};
    }

    indices_.reserve(asset.indices().size());
    std::transform(std::begin(asset.indices()), std::end(asset.indices()), std::back_inserter(indices_), [](const uint32_t index) {
       return static_cast<uint16_t>(index);
    });
}

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

void StaticMeshDefinition::add(vertex::Indice indice) {
    indices_.push_back(indice);
}

void StaticMeshDefinition::add_triangle_indices(vertex::Indice a, vertex::Indice b, vertex::Indice c) {
    add(a);
    add(b);
    add(c);
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

bool StaticMeshDefinition::uses_indices() const noexcept {
    return !indices_.empty();
}

const std::vector<vertex::Position>& StaticMeshDefinition::positions() const noexcept {
    return positions_;
}

const std::vector<vertex::Indice>& StaticMeshDefinition::indices() const noexcept {
    return indices_;
}

}