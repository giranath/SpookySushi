#ifndef SPOOKYSUSHI_PHYSIC_WORLD_TRAITS_HPP
#define SPOOKYSUSHI_PHYSIC_WORLD_TRAITS_HPP

#include "types.hpp"
#include "rigid_body_traits.hpp"

#include <memory>

namespace sushi {

template<typename T>
struct physic_world_traits {
    using value_type = T;
    using rigid_body_type = void;
    using joint_type = void;

    static const bool can_draw_debug = false;
};

}

#endif //SPOOKYSUSHI_PHYSIC_WORLD_HPP
