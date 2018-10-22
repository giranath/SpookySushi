#include "physx_joint.hpp"

#include <extensions/PxJoint.h>

namespace sushi {

PhysXJoint::PhysXJoint(physx::PxJoint *joint)
: joint{joint} {

}

}
