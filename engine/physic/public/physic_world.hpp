#ifndef SPOOKYSUSHI_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_PHYSIC_WORLD_HPP

#include "types.hpp"

#include <memory>

namespace sushi {

class PhysicWorld {
public:
    virtual ~PhysicWorld() = default;

    virtual void set_gravity(const Vec3& gravity) = 0;

    virtual void step_simulation(float dt) = 0;
};

std::unique_ptr<PhysicWorld> make_physic_world();

}

#endif //SPOOKYSUSHI_PHYSIC_WORLD_HPP
