#ifndef SPOOKYSUSHI_MESH_HPP
#define SPOOKYSUSHI_MESH_HPP

#include "../core/types.hpp"
#include <vector>
#include <stdexcept>

namespace sushi {

class invalid_mesh_error : public std::runtime_error {
public:
    invalid_mesh_error() : std::runtime_error{"invalid mesh"} {}
};

class mesh {
    std::vector<vec3> vertices_;
    std::vector<vec3> normals_;
    std::vector<vec2> uvs_;
public:
    /**
     * Construct an empty mesh
     */
    mesh() noexcept = default;

    /**
     * Construct a mesh
     */
    explicit mesh(const std::vector<vec3>& vertices);
    mesh(const std::vector<vec3>& vertices, const std::vector<vec2>& uvs);
    mesh(const std::vector<vec3>& vertices, const std::vector<vec3>& normals);
    mesh(const std::vector<vec3>& vertices, const std::vector<vec2>& uvs, const std::vector<vec3>& normals);

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
     * Clear the content of this mesh to make it an empty mesh
     */
    void clear() noexcept;

    /**
     * Returns the number of vertices contained in this mesh
     * @return The number of vertices in this mesh
     */
    std::size_t size() const noexcept;

    const std::vector<vec3>& vertices() const noexcept;
    const std::vector<vec2>& uvs() const noexcept;
    const std::vector<vec3>& normals() const noexcept;
};

}

#endif
