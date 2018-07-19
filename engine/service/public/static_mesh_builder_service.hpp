#ifndef SPOOKYSUSHI_STATIC_MESH_BUILDER_SERVICE_HPP
#define SPOOKYSUSHI_STATIC_MESH_BUILDER_SERVICE_HPP

#include "service_locator.hpp"

namespace sushi {

class StaticMeshBuilder;

class StaticMeshBuilderService : public ServiceLocator<StaticMeshBuilder> {};

}

#endif //SPOOKYSUSHI_STATIC_MESH_BUILDER_SERVICE_HPP
