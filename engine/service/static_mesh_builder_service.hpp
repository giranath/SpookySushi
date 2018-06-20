#ifndef SPOOKYSUSHI_STATIC_MESH_BUILDER_SERVICE_HPP
#define SPOOKYSUSHI_STATIC_MESH_BUILDER_SERVICE_HPP


namespace sushi {

class StaticMeshBuilder;

class StaticMeshBuilderService {
    static StaticMeshBuilder* builder;
public:
    static void locate(StaticMeshBuilder* builder) noexcept;
    static StaticMeshBuilder& get() noexcept;
};
}

#endif //SPOOKYSUSHI_STATIC_MESH_BUILDER_SERVICE_HPP
