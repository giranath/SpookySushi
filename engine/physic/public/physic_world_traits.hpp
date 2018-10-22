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

#if 0
class PhysicWorld {
public:
    virtual ~PhysicWorld() = default;

    virtual void set_gravity(const Vec3& gravity) = 0;

    virtual void step_simulation(float dt) = 0;

    virtual void draw_debug_informations() = 0;
};

std::unique_ptr<PhysicWorld> make_physic_world();
#endif

}

#endif //SPOOKYSUSHI_PHYSIC_WORLD_HPP
