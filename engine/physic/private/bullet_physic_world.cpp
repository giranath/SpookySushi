#include "bullet_physic_world.hpp"
#include "bullet_debug_drawer.hpp"

#include <btBulletDynamicsCommon.h>

#include <cassert>

namespace sushi {

BulletPhysicWorld::BulletPhysicWorld()
: collision_configurations{std::make_unique<btDefaultCollisionConfiguration>()}
, collision_dispatcher{std::make_unique<btCollisionDispatcher>(collision_configurations.get())}
, overlapping_pair_cache{std::make_unique<btDbvtBroadphase>()}
, solver{std::make_unique<btSequentialImpulseConstraintSolver>()}
, dynamic_world{std::make_unique<btDiscreteDynamicsWorld>(collision_dispatcher.get(), overlapping_pair_cache.get(), solver.get(), collision_configurations.get())}
, debug_draw{std::make_unique<BulletDebugDrawer>()}{
    dynamic_world->setGravity(btVector3{0.f, -9.8f, 0.f});

    debug_draw->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
    dynamic_world->setDebugDrawer(debug_draw.get());
}

BulletPhysicWorld::~BulletPhysicWorld() = default;

void BulletPhysicWorld::step_simulation(float dt_ms) {
    dynamic_world->stepSimulation(dt_ms);
}

static btTransform to_bullet_transform(const PhysicTransform& transform) {
    btTransform origin_transform;
    origin_transform.setIdentity();
    origin_transform.setOrigin(btVector3(transform.translation.x, transform.translation.y, transform.translation.z));
    origin_transform.setRotation(btQuaternion(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w));

    return origin_transform;
}

static btRigidBody* make_bullet_rigid_body(const PhysicTransform& transform, btCollisionShape* shape, float mass) {
    const btTransform origin_transform = to_bullet_transform(transform);
    btScalar b_mass = static_cast<btScalar>(mass);

    const bool is_dynamic = (mass != 0.f);

    btVector3 local_inertia(0.f, 0.f, 0.f);
    if(is_dynamic) {
        shape->calculateLocalInertia(b_mass, local_inertia);
    }

    btDefaultMotionState* motion_state = new btDefaultMotionState(origin_transform);
    btRigidBody::btRigidBodyConstructionInfo infos(b_mass, motion_state, shape, local_inertia);

    return new btRigidBody(infos);
}

BulletPhysicWorld::rigid_body_type BulletPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass) {
    btCollisionShape* b_shape = new btSphereShape(shape.radius);

    btRigidBody* body = make_bullet_rigid_body(transform, b_shape, mass);

    // Add the rigibody to the world
    dynamic_world->addRigidBody(body);

    return rigid_body_type{body};
}

BulletPhysicWorld::rigid_body_type BulletPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass) {
    btCollisionShape* b_shape = new btBoxShape(btVector3(shape.width, shape.height, shape.depth));

    btRigidBody* body = make_bullet_rigid_body(transform, b_shape, mass);

    // Add the rigibody to the world
    dynamic_world->addRigidBody(body);

    return rigid_body_type{body};
}

BulletPhysicWorld::rigid_body_type BulletPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass) {
    assert(shape.axis == PhysicCapsuleShape::AxisX
        || shape.axis == PhysicCapsuleShape::AxisY
        || shape.axis == PhysicCapsuleShape::AxisZ);

    btCollisionShape* b_shape = nullptr;
    switch(shape.axis) {
        case PhysicCapsuleShape::AxisY:
            b_shape = new btCapsuleShape(shape.radius, shape.height);
            break;
        case PhysicCapsuleShape::AxisX:
            b_shape = new btCapsuleShapeX(shape.radius, shape.height);
            break;
        case PhysicCapsuleShape::AxisZ:
            b_shape = new btCapsuleShapeZ(shape.radius, shape.height);
            break;
        default:
            throw std::out_of_range("specified capsule axis is out of range");
    }

    btRigidBody* body = make_bullet_rigid_body(transform, b_shape, mass);

    // Add the rigibody to the world
    dynamic_world->addRigidBody(body);

    return rigid_body_type{body};
}

void BulletPhysicWorld::draw_debug() const {
    dynamic_world->debugDrawWorld();
}

}