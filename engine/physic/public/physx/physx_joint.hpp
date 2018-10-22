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

};

}

#endif //SPOOKYSUSHI_PHYSX_JOINT_HPP
