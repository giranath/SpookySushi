#ifndef SPOOKYSUSHI_STATIC_MESH_BUILDER_HPP
#define SPOOKYSUSHI_STATIC_MESH_BUILDER_HPP

#include "../core/types.hpp"
#include <vector>
#include <memory>

namespace sushi {

namespace vertex {
using Position = Vec3;
using Normal = Vec3;
using UV = Vec2;
}

/**
 * Defines a static mesh
 */
class StaticMeshDefinition {
    std::vector<vertex::Position> positions_;
    std::vector<vertex::Normal> normals_;
    std::vector<vertex::UV> uvs_;
public:
    /**
     * Clear the definition to represent a mesh with no vertices
     */
    void clear();

    /**
     * Add a vertex
     * @param position The position of the vertex
     */
    void add(const vertex::Position& position);

    /**
     * Add a vertex
     * @param position The position of the vertex
     * @param uv The UV mapping of the vertex
     */
    void add(const vertex::Position& position, const vertex::UV& uv);

    /**
     * Add a vertex
     * @param position The position of the vertex
     * @param normal The normal of the vertex
     */
    void add(const vertex::Position& position, const vertex::Normal& normal);

    /**
     * Add a vertex
     * @param position The position of the vertex
     * @param uv The uv mapping of the vertex
     * @param normal The normal of the vertex
     */
    void add(const vertex::Position& position, const vertex::UV& uv, const vertex::Normal& normal);

    /**
     * Checks if the definition is correct
     * @return true if the definition is valid, false otherwise
     */
    bool good() const noexcept;

    /**
     * Checks if the defined mesh uses normals
     * @return true if each vertex has a normal
     */
    bool uses_normals() const noexcept;

    /**
     * Check if the defined mesh uses UV mapping
     * @return true if each vertex has a UV coordinate
     */
    bool uses_uvs() const noexcept;

    const std::vector<vertex::Position>& positions() const noexcept;
};

class StaticMeshTooLargeError : public std::runtime_error {
public:
    StaticMeshTooLargeError() : std::runtime_error{"static mesh too large"} {

    }
};

/**
 * Interface of a static mesh
 */
class StaticMesh {
public:
    virtual ~StaticMesh() noexcept = default;

    /**
     * Called to render the current static mesh on screen
     */
    virtual void render() = 0;
};

using StaticMeshPtr = std::unique_ptr<StaticMesh>;

/**
 * Build static meshes
 */
class StaticMeshBuilder {
public:
    /**
     * Build a static mesh from it's definition
     * @param definition The static mesh's definition
     * @return The created static mesh
     */
    virtual StaticMeshPtr build(const StaticMeshDefinition& definition) = 0;
};
}

#endif //SPOOKYSUSHI_STATIC_MESH_BUILDER_HPP
