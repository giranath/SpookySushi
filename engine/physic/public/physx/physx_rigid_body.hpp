#ifndef SPOOKYSUSHI_PHYSX_RIGID_BODY_HPP
#define SPOOKYSUSHI_PHYSX_RIGID_BODY_HPP

#include "rigid_body_traits.hpp"
#include "physic_transform.hpp"
#include <types.hpp>
#include <physic_movement_lock.hpp>

namespace physx {

class PxRigidActor;

}

namespace sushi {

class PhysXPhysicWorld;
class PhysXJoint;
struct RigidBodyNotBound {};

class PhysXRigidBody {
    friend PhysXPhysicWorld;
    friend PhysXJoint;
    physx::PxRigidActor* rigid_body;

    explicit PhysXRigidBody(physx::PxRigidActor* rigid_body) noexcept;
public:
    PhysXRigidBody() noexcept;

    explicit operator bool() const noexcept { return good(); }
    bool good() const noexcept { return rigid_body; }

    void set_linear_damping(float damp);
    void set_angular_damping(float damp);
    void apply_force_at(const Vec3& position, const Vec3& force);
    PhysicTransform transform() const;

    void set_query_filter_mask(uint32_t filter_mask);
    uint32_t query_filter_mask() const;

    void set_movement_lock(MovementLock lock);
};

template<>
struct is_rigid_body<PhysXRigidBody> {
    static const bool value = true;
};

}

#endif //SPOOKYSUSHI_PHYSX_RIGID_BODY_HPP
