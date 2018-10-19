#ifndef SPOOKYSUSHI_PHYSX_RIGID_BODY_HPP
#define SPOOKYSUSHI_PHYSX_RIGID_BODY_HPP

#include "rigid_body_traits.hpp"

namespace sushi {

class PhysXPhysicWorld;

class PhysXRigidBody {
    friend PhysXPhysicWorld;
public:
    explicit operator bool() const noexcept { return false; }
};

template<>
struct is_rigid_body<PhysXRigidBody> {
    static const bool value = true;
};

}

#endif //SPOOKYSUSHI_PHYSX_RIGID_BODY_HPP
