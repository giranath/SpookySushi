#include "mesh_asset.hpp"

#include <algorithm>

namespace sushi {

MeshAsset::MeshAsset() = default;

MeshAsset::MeshAsset(const sushi::MeshAsset::vertex_array &vertices, const sushi::MeshAsset::normal_array &normals,
                     const sushi::MeshAsset::texture_coord_array &uvs, const sushi::MeshAsset::index_array &indices)
: vertices_{vertices}
, normals_{normals}
, texture_coords_{uvs}
, indices_{indices} {

}

void MeshAsset::swap(MeshAsset& other) noexcept {
    using std::swap;

    swap(vertices_, other.vertices_);
    swap(normals_, other.normals_);
    swap(texture_coords_, other.texture_coords_);
    swap(indices_, other.indices_);
}

static const uint8_t CURRENT_SERIALIZATION_VERSION = 1;

std::ostream& operator<<(std::ostream& stream, const MeshAsset& asset) {
    stream.write(reinterpret_cast<const char *>(&CURRENT_SERIALIZATION_VERSION), sizeof(uint8_t));

    const uint32_t vertices_count = static_cast<uint32_t>(asset.vertices_.size());
    const bool needs_large_index = vertices_count > std::numeric_limits<uint16_t>::max();
    uint8_t flags = mesh_asset::FLAGS_NONE;

    if(needs_large_index) {
        flags |= mesh_asset::FLAGS_LARGE_INDEX;
    }

    if(asset.has_normals()) {
        flags |= mesh_asset::FLAGS_NORMALS;
    }

    if(asset.is_indexed()) {
        flags |= mesh_asset::FLAGS_IS_INDEXED;
    }

    if(asset.has_texture()) {
        flags |= mesh_asset::FLAGS_TEXTURE_COORDS;
    }

    stream.write(reinterpret_cast<const char*>(&flags), sizeof(uint8_t));
    stream.write(reinterpret_cast<const char*>(&vertices_count), sizeof(uint32_t));
    stream.write(reinterpret_cast<const char*>(asset.vertices_.data()), sizeof(float) * 3 * vertices_count);

    if((flags & mesh_asset::FLAGS_NORMALS) != 0) {
        stream.write(reinterpret_cast<const char*>(asset.normals_.data()), sizeof(float) * 3 * vertices_count);
    }

    if((flags & mesh_asset::FLAGS_TEXTURE_COORDS) != 0) {
        stream.write(reinterpret_cast<const char*>(asset.texture_coords_.data()), sizeof(float) * 2 * vertices_count);
    }

    if((flags & mesh_asset::FLAGS_IS_INDEXED) != 0) {
        const uint32_t index_count = static_cast<uint32_t>(asset.indices_.size());

        stream.write(reinterpret_cast<const char*>(&index_count), sizeof(uint32_t));

        if((flags & mesh_asset::FLAGS_LARGE_INDEX) == 0) {
            for(uint32_t index : asset.indices_) {
                const uint16_t small_index = static_cast<const uint16_t>(index);
                stream.write(reinterpret_cast<const char*>(&small_index), sizeof(uint16_t));
            }
        }
        else {
            stream.write(reinterpret_cast<const char*>(asset.indices_.data()), sizeof(uint32_t) * index_count);
        }
    }

    return stream;
}

std::istream& operator>>(std::istream& stream, MeshAsset& asset) {
    asset = MeshAsset{};

    uint8_t version;
    stream.read(reinterpret_cast<char*>(&version), sizeof(uint8_t));

    if(version != CURRENT_SERIALIZATION_VERSION) {
        stream.setstate(std::ios::failbit);
        return stream;
    }

    uint8_t flags;
    stream.read(reinterpret_cast<char*>(&flags), sizeof(uint8_t));

    uint32_t vertices_count;
    stream.read(reinterpret_cast<char*>(&vertices_count), sizeof(uint32_t));

    for(uint32_t i = 0; i < vertices_count; ++i) {
        asset.vertices_.reserve(vertices_count);
        Vec3 vertex;
        stream.read(reinterpret_cast<char*>(&vertex), sizeof(Vec3));

        asset.vertices_.push_back(vertex);
    }

    if((flags & mesh_asset::FLAGS_NORMALS) != 0) {
        asset.normals_.reserve(vertices_count);
        for(uint32_t i = 0; i < vertices_count; ++i) {
            Vec3 normal;
            stream.read(reinterpret_cast<char*>(&normal), sizeof(Vec3));

            asset.normals_.push_back(normal);
        }
    }

    if((flags & mesh_asset::FLAGS_TEXTURE_COORDS) != 0) {
        asset.texture_coords_.reserve(vertices_count);
        for(uint32_t i = 0; i < vertices_count; ++i) {
            Vec2 uv;
            stream.read(reinterpret_cast<char*>(&uv), sizeof(Vec2));

            asset.texture_coords_.push_back(uv);
        }
    }

    if((flags & mesh_asset::FLAGS_IS_INDEXED) != 0) {
        uint32_t index_count;
        stream.read(reinterpret_cast<char*>(&index_count), sizeof(uint32_t));

        asset.indices_.reserve(index_count);

        for(uint32_t i = 0; i < index_count; ++i) {
            uint32_t indice;

            if((flags & mesh_asset::FLAGS_LARGE_INDEX) == 0) {
                uint16_t small_index;
                stream.read(reinterpret_cast<char*>(&small_index), sizeof(uint16_t));

                indice = small_index;
            }
            else {
                stream.read(reinterpret_cast<char*>(&indice), sizeof(uint32_t));
            }

            asset.indices_.push_back(indice);
        }
    }

    return stream;
}

}