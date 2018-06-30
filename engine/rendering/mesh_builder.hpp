#ifndef SPOOKYSUSHI_MESH_BUILDER_HPP
#define SPOOKYSUSHI_MESH_BUILDER_HPP

#include "mesh.hpp"

namespace sushi {

class MeshBuilder {
    std::vector<Vec3> positions;
    std::vector<Vec2> uvs;
    std::vector<Vec3> normals;
public:
    void add_vertex(const Vec3& position);
    void add_vertex(const Vec3& position, const Vec2& uv);
    void add_vertex(const Vec3& position, const Vec3& normal);
    void add_vertex(const Vec3& position, const Vec2& uv, const Vec3& normal);

    void clear() noexcept;
    Mesh build() const noexcept;
};

}

#endif
