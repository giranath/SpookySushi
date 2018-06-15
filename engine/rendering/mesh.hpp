#ifndef SPOOKYSUSHI_MESH_HPP
#define SPOOKYSUSHI_MESH_HPP

#include "../core/types.hpp"
#include <vector>

namespace sushi {

class mesh {
    std::vector<vec3> vertices;
    std::vector<vec3> normals;
    std::vector<vec2> uvs;
public:
    /**
     * Construct an empty mesh
     */
    mesh() noexcept = default;

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

    void push_vertex(glm::vec3 vertex);
    void push_vertex(glm::vec3 vertex, glm::vec2 uv);
    void push_vertex(glm::vec3 vertex, glm::vec3 normal);
    void push_vertex(glm::vec3 vertex, glm::vec2 uv, glm::vec3 normal);
};

}

#endif
