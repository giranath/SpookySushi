#ifndef SPOOKYSUSHI_MESH_HPP
#define SPOOKYSUSHI_MESH_HPP

#include "types.hpp"
#include <vector>
#include <stdexcept>

namespace sushi {

class InvalidMeshError : public std::runtime_error {
public:
    InvalidMeshError() : std::runtime_error{"invalid mesh"} {}
};

class Mesh {
    std::vector<Vec3> vertices_;
    std::vector<Vec3> normals_;
    std::vector<Vec2> uvs_;
public:
    /**
     * Construct an empty mesh
     */
    Mesh() noexcept = default;

    /**
     * Construct a mesh
     */
    explicit Mesh(const std::vector<Vec3>& vertices);
    Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs);
    Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec3>& normals);
    Mesh(const std::vector<Vec3>& vertices, const std::vector<Vec2>& uvs, const std::vector<Vec3>& normals);

    /**
     * Check if this mesh uses uv coordinates
     * @return true if uv coordinates are used for this mesh
     */
    bool use_uvs() const noexcept;

    /**
     * Check if this mesh uses normals
     * @return true if normals are used for this mesh
     */
    bool use_normals() const noexcept;

    /**
     * Returns the number of vertices contained in this mesh
     * @return The number of vertices in this mesh
     */
    std::size_t size() const noexcept;

    const std::vector<Vec3>& vertices() const noexcept;
    const std::vector<Vec2>& uvs() const noexcept;
    const std::vector<Vec3>& normals() const noexcept;
};

}

#endif
