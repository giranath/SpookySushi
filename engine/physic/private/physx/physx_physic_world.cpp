#include "physx_physic_world.hpp"

namespace sushi {

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