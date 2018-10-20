#include "physx_physics.h"

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxSceneDesc.h>

using namespace physx;

namespace sushi {

PhysXPhysics::PhysXPhysics(PxPhysics* physics) noexcept
: physics{physics} {
}

PhysXPhysics::~PhysXPhysics() noexcept {
    physics->release();
}

PxScene* PhysXPhysics::make_scene() {
    PxSceneDesc scene_description{PxTolerancesScale{}};
    scene_description.gravity = PxVec3{0.f, -9.8f, 0.f};

    return physics->createScene(scene_description);
}

}