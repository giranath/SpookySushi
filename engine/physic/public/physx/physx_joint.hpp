#ifndef SPOOKYSUSHI_PHYSX_JOINT_HPP
#define SPOOKYSUSHI_PHYSX_JOINT_HPP

#include "physx_rigid_body.hpp"

namespace physx {

class PxJoint;

}

namespace sushi {

class PhysXPhysicWorld;

class PhysXJoint {
    friend PhysXPhysicWorld;
    physx::PxJoint* joint;

    PhysXJoint(physx::PxJoint* joint);
public:
    PhysXJoint() noexcept;

    operator bool() const noexcept { return good(); }
    bool good() const noexcept { return joint; }

    bool is_collision_enabled() const noexcept;
    void enable_collision() noexcept;
    void disable_collision() noexcept;

    std::pair<PhysXRigidBody, PhysXRigidBody> rigid_bodies() noexcept;
};

}

#endif //SPOOKYSUSHI_PHYSX_JOINT_HPP
