#include "bullet_joint.hpp"

#include <btBulletDynamicsCommon.h>

namespace sushi {

bool BulletJoint::good() const noexcept {
    return constraints != nullptr;
}

bool BulletJoint::is_collision_enabled() const noexcept {
    return false;
}

void BulletJoint::enable_collision() noexcept {

}

void BulletJoint::disable_collision() noexcept {

}

std::pair<BulletRigidBody, BulletRigidBody> BulletJoint::rigid_bodies() noexcept {
    return std::make_pair(BulletRigidBody(&constraints->getRigidBodyA()), BulletRigidBody(&constraints->getRigidBodyB()));
}

}