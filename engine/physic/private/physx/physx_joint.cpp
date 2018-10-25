#include "physx_joint.hpp"

#include <extensions/PxJoint.h>
#include <PxConstraint.h>

namespace sushi {

PhysXJoint::PhysXJoint(physx::PxJoint *joint)
: joint{joint} {

}

PhysXJoint::PhysXJoint() noexcept
: joint{ nullptr } {
}

bool PhysXJoint::is_collision_enabled() const noexcept {
    return joint->getConstraintFlags().isSet(physx::PxConstraintFlag::eCOLLISION_ENABLED);
}

void PhysXJoint::enable_collision() noexcept {
    joint->setConstraintFlag(physx::PxConstraintFlag::eCOLLISION_ENABLED, true);
}

void PhysXJoint::disable_collision() noexcept {
    joint->setConstraintFlag(physx::PxConstraintFlag::eCOLLISION_ENABLED, false);
}

std::pair<PhysXRigidBody, PhysXRigidBody> PhysXJoint::rigid_bodies() noexcept {
    physx::PxRigidActor *a, *b;
    joint->getActors(a, b);

    return std::make_pair(PhysXRigidBody(a), PhysXRigidBody(b));
}

}
