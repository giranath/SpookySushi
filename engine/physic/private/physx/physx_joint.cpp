#include "physx_joint.hpp"

#include <extensions/PxJoint.h>
#include <PxConstraint.h>

namespace sushi {

PhysXJoint::PhysXJoint(physx::PxJoint *joint)
: joint{joint} {

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

}
