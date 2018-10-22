#ifndef SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP

#include "physic_world_traits.hpp"
#include "physic_shape.hpp"
#include "physic_joints.hpp"
#include "physic_transform.hpp"
#include "raycast_result.hpp"

#include "physx/physx_rigid_body.hpp"
#include "physx/physx_joint.hpp"

#include <memory>

namespace sushi {

class PhysXPhysicWorld {
    struct impl;

    std::unique_ptr<impl> pimpl;

public:
    using rigid_body_type = PhysXRigidBody;
    using joint_type = PhysXJoint;
    using raycast_result = RayCastResult<rigid_body_type>;

    PhysXPhysicWorld();
    ~PhysXPhysicWorld();

    void step_simulation(float dt_ms);

    // Rigid Body
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicPlaneShape shape, float mass = 0.f);

    // Joints
    joint_type join(rigid_body_type a, rigid_body_type b, PhysicRopeJoint joint);

    void destroy(rigid_body_type& body);
    void destroy(joint_type& join);

    // Scene queries
    raycast_result raycast(const Vec3& starting_point, const Vec3& direction, float max_distance);

    void draw_debug() const;
};

template<>
struct physic_world_traits<PhysXPhysicWorld> {
    using value_type = PhysXPhysicWorld;
    using rigid_body_type = PhysXPhysicWorld::rigid_body_type;
    using joint_type = PhysXPhysicWorld::joint_type;

    static const bool can_draw_debug = true;
};

}

#endif //SPOOKYSUSHI_PHYSX_PHYSIC_WORLD_HPP
