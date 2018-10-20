#include "physx_foundation.hpp"

#include <PxPhysics.h>
#include <PxPhysicsVersion.h>
#include <foundation/PxFoundationVersion.h>
#include <foundation/PxFoundation.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>
#include <common/PxTolerancesScale.h>

using namespace physx;

namespace sushi {

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;

PhysXFoundation::PhysXFoundation()
: foundation(PxCreateFoundation(PX_FOUNDATION_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback)){
    if(!foundation) {
        throw CannotCreatePhysXFoundation{};
    }
}

PhysXFoundation::~PhysXFoundation() {
    foundation->release();
}

PhysXPhysics PhysXFoundation::make_physics() const {
    PxPhysics* phs = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, PxTolerancesScale{});
    if(!phs) {
        throw CannotCreatePhysXPhysics{};
    }

    return PhysXPhysics(phs);
}

}