#ifndef SPOOKYSUSHI_PHYSX_JOINT_HPP
#define SPOOKYSUSHI_PHYSX_JOINT_HPP

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

    operator bool() const noexcept { return good(); }
    bool good() const noexcept { return joint; }

    bool is_collision_enabled() const noexcept;
    void enable_collision() noexcept;
    void disable_collision() noexcept;

};

}

#endif //SPOOKYSUSHI_PHYSX_JOINT_HPP
