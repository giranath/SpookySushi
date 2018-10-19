#ifndef SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP

#include "physic_world_traits.hpp"
#include "physic_shape.hpp"
#include "physic_transform.hpp"

#include "physx/physx_rigid_body.hpp"

namespace sushi {

class PhysXPhysicWorld {
public:
    using rigid_body_type = PhysXRigidBody;

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
