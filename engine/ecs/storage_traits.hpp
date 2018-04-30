#ifndef SPOOKYSUSHI_STORAGE_TRAITS_HPP
#define SPOOKYSUSHI_STORAGE_TRAITS_HPP

#include <memory>

namespace sushi { namespace ecs {

template<typename T>
struct storage_traits {
    using component_type = void;
};

}}

#endif //SPOOKYSUSHI_STORAGE_TRAITS_HPP
