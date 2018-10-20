#include "physx_foundation.hpp"

#include <foundation/PxFoundationVersion.h>
#include <foundation/PxFoundation.h>
#include <extensions/PxDefaultAllocator.h>
#include <extensions/PxDefaultErrorCallback.h>

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
}