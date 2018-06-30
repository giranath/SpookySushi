#include "static_mesh_builder_service.hpp"
#include "../rendering/static_mesh_builder.hpp"

namespace sushi {

StaticMeshBuilder* StaticMeshBuilderService::builder = nullptr;

void StaticMeshBuilderService::locate(StaticMeshBuilder* b) noexcept {
    builder = b;
}

StaticMeshBuilder& StaticMeshBuilderService::get() noexcept {
    return *builder;
}

}