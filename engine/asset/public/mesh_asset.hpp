#ifndef SPOOKYSUSHI_MESH_ASSET_HPP
#define SPOOKYSUSHI_MESH_ASSET_HPP

#include <types.hpp>
#include <cstdint>
#include <vector>
#include <iostream>

namespace sushi {

enum class MeshType : uint8_t {
    StaticMesh,              // Not indexed
    IndexedStaticMesh,       // 16 bit index
    LargeIndexedStaticMesh,  // 32 bit index
    Unknown
};

namespace mesh_asset {
enum MeshFlagBits : uint8_t {
    FLAGS_NONE =            0,
    FLAGS_LARGE_INDEX =     1,
    FLAGS_NORMALS =        (1 << 1),
    FLAGS_IS_INDEXED =     (1 << 2),
    FLAGS_VERTEX_COLOR =   (1 << 3),
    FLAGS_TEXTURE_COORDS = (1 << 4)
};
}

class MeshAsset {
public:
    using vertex_array = std::vector<Vec3>;
    using normal_array = std::vector<Vec3>;
    using texture_coord_array = std::vector<Vec2>;
    using index_array = std::vector<uint32_t>;
private:
    vertex_array vertices_;
    normal_array normals_;
    texture_coord_array texture_coords_;
    index_array indices_;
public:
    MeshAsset();
    MeshAsset(const vertex_array& vertices, const normal_array& normals, const texture_coord_array& uvs, const index_array& indices);

    bool has_normals() const noexcept { return !normals_.empty(); }
    bool has_texture() const noexcept { return !texture_coords_.empty(); }
    bool is_indexed() const noexcept { return !indices_.empty(); }

    const vertex_array& vertices() const noexcept { return vertices_; }
    const normal_array& normals() const noexcept { return normals_; }
    const texture_coord_array& texture_coords() const noexcept { return texture_coords_; }
    const index_array& indices() const noexcept { return indices_; }

    void swap(MeshAsset& other) noexcept;

    friend std::ostream& operator<<(std::ostream& stream, const MeshAsset& asset);
    friend std::istream& operator>>(std::istream& stream, MeshAsset& asset);
};

}

#endif //SPOOKYSUSHI_MESH_ASSET_HPP
