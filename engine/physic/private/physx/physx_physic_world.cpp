#include "physx_physic_world.hpp"
#include "physx_foundation.hpp"

#include <PxPhysics.h>
#include <PxPhysicsVersion.h>
#include <common/PxTolerancesScale.h>

namespace sushi {

struct PhysXPhysicWorld::impl {
    PhysXFoundation foundation;
    physx::PxPhysics* physics;

    impl()
    : foundation{}
    , physics{PxCreatePhysics(PX_PHYSICS_VERSION, foundation.get(), physx::PxTolerancesScale())}{
    }

    ~impl() {
        physics->release();
    }
};

PhysXPhysicWorld::PhysXPhysicWorld()
: pimpl(std::make_unique<PhysXPhysicWorld::impl>()) {

}

PhysXPhysicWorld::~PhysXPhysicWorld() = default;

void PhysXPhysicWorld::step_simulation(float dt_ms) {

}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass) {
    return {};
}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass) {
    return {};
}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass) {
    return {};
}

void PhysXPhysicWorld::draw_debug() const {
    // TODO: Draw debug informations
}

}