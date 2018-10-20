#ifndef SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP

#include "physic_world_traits.hpp"
#include "physic_shape.hpp"
#include "physic_transform.hpp"

#include "physx/physx_rigid_body.hpp"

#include <memory>

namespace sushi {

class PhysXPhysicWorld {
    struct impl;

    std::unique_ptr<impl> pimpl;

public:
    using rigid_body_type = PhysXRigidBody;

    PhysXPhysicWorld();
    ~PhysXPhysicWorld();

    void step_simulation(float dt_ms);

    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass = 0.f);

    void draw_debug() const;
};

template<>
struct physic_world_traits<PhysXPhysicWorld> {
    using value_type = PhysXPhysicWorld;
    using rigid_body_type = PhysXPhysicWorld::rigid_body_type;

    static const bool can_draw_debug = true;
};

}

#endif //SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP
