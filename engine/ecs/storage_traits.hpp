#ifndef SPOOKYSUSHI_STORAGE_TRAITS_HPP
#define SPOOKYSUSHI_STORAGE_TRAITS_HPP

#include <memory>

namespace sushi { namespace ecs {

template<typename T>
struct storage_traits {
    using component_type = void;
    using iterator = void;
    using const_iterator = void;
};

}}

#endif //SPOOKYSUSHI_STORAGE_TRAITS_HPP
