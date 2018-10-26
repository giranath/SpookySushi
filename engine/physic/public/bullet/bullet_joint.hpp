#ifndef SPOOKYSUSHI_BULLET_JOINT_HPP
#define SPOOKYSUSHI_BULLET_JOINT_HPP

#include "bullet_rigid_body.hpp"

class btTypedConstraint;

namespace sushi {

class BulletPhysicWorld;

class BulletJoint {
    friend BulletPhysicWorld;
    btTypedConstraint* constraints;

    explicit BulletJoint(btTypedConstraint* constraints) : constraints{constraints} {

    }

public:
    BulletJoint() noexcept : constraints{} {

    }

    operator bool() const noexcept { return good(); }

    bool good() const noexcept;

    bool is_collision_enabled() const noexcept;
    void enable_collision() noexcept;
    void disable_collision() noexcept;

    std::pair<BulletRigidBody, BulletRigidBody> rigid_bodies() noexcept;

};

}

#endif //SPOOKYSUSHI_BULLET_JOINT_HPP
