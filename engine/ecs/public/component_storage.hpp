#ifndef SPOOKYSUSHI_COMPONENT_STORAGE_HPP
#define SPOOKYSUSHI_COMPONENT_STORAGE_HPP

#include "entity.hpp"
#include <type_traits>

namespace sushi {

template<typename StorageType>
struct component_storage_traits {
    using component_type = void;
    using iterator = void;
    using const_iterator = void;
};

template<typename StorageType>
struct is_component_storage {
    static const bool value = !std::is_same_v<component_storage_traits<StorageType>::component_type, void>;
};

template<typename StorageType>
constexpr bool is_component_storage_v = is_component_storage<StorageType>::value;

}

#endif
