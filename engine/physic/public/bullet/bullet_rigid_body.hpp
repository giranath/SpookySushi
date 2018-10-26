#ifndef SPOOKYSUSHI_BULLET_RIGID_BODY_HPP
#define SPOOKYSUSHI_BULLET_RIGID_BODY_HPP

#include "rigid_body_traits.hpp"
#include "physic_transform.hpp"
#include "physic_movement_lock.hpp"

class btRigidBody;

namespace sushi {

class BulletPhysicWorld;
class BulletJoint;

class BulletRigidBody {
    friend BulletPhysicWorld;
    friend BulletJoint;

    btRigidBody* body;

    explicit BulletRigidBody(btRigidBody* body) noexcept;
public:
    BulletRigidBody() noexcept;

    explicit operator bool() const noexcept;

    void set_linear_damping(float damp);
    void set_angular_damping(float damp);
    void apply_force_at(const Vec3& position, const Vec3& force);
    PhysicTransform transform() const;

    void set_query_filter_mask(uint32_t filter_mask);
    uint32_t query_filter_mask() const;

    void set_movement_lock(MovementLock lock);
};

template<>
struct is_rigid_body<BulletRigidBody> {
    static const bool value = true;
};

}

#endif
