#ifndef SPOOKYSUSHI_MAP_STORAGE_HPP
#define SPOOKYSUSHI_MAP_STORAGE_HPP

#include "component_storage.hpp"
#include "entity.hpp"
#include <map>

namespace sushi {

template<typename ComponentType>
using MapStorage = std::map<Entity, ComponentType>;

template<typename ComponentType>
struct component_storage_traits<MapStorage<ComponentType>> {
    using component_type = ComponentType;
    using iterator = typename std::map<Entity, ComponentType>::iterator;
    using const_iterator = typename std::map<Entity, ComponentType>::const_iterator;
};

}

#endif
