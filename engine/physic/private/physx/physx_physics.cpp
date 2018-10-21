#include "physx_physics.h"

#include <PxPhysics.h>
#include <PxScene.h>
#include <PxSceneDesc.h>
#include <extensions/PxDefaultSimulationFilterShader.h>
#include <extensions/PxDefaultCpuDispatcher.h>

using namespace physx;

namespace sushi {

PhysXPhysics::PhysXPhysics(PxPhysics* physics) noexcept
: physics{physics}
, cpu_dispatcher(PxDefaultCpuDispatcherCreate(2)){
}

PhysXPhysics::~PhysXPhysics() noexcept {
    cpu_dispatcher->release();
    physics->release();
}

PxScene* PhysXPhysics::make_scene() {
    PxSceneDesc scene_description{physics->getTolerancesScale()};
    scene_description.gravity = PxVec3{0.f, -9.8f, 0.f};
    scene_description.cpuDispatcher = cpu_dispatcher;
    scene_description.filterShader	= PxDefaultSimulationFilterShader;

    return physics->createScene(scene_description);
}

}