#include "bullet_physic_world.hpp"
#include "bullet_debug_drawer.hpp"
#include <types.hpp>

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

    debug_draw->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb | btIDebugDraw::DBG_DrawContactPoints);
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

BulletPhysicWorld::rigid_body_type BulletPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicPlaneShape shape) {
    btCollisionShape* b_shape = new btStaticPlaneShape(btVector3{shape.normal.x, shape.normal.y, shape.normal.z},
                                                       shape.distance);

    btRigidBody* body = make_bullet_rigid_body(transform, b_shape, 0.f);
    dynamic_world->addRigidBody(body);

    return rigid_body_type{body};
}

// Joints
BulletPhysicWorld::joint_type BulletPhysicWorld::join(rigid_body_type a, rigid_body_type b, PhysicRopeJoint joint) {
    throw 5;
    return joint_type{};
}

void BulletPhysicWorld::destroy(rigid_body_type& body) {
    dynamic_world->removeRigidBody(body.body);
    body.body = nullptr;
}

void BulletPhysicWorld::destroy(joint_type& join) {
    dynamic_world->removeConstraint(join.constraints);
    join.constraints = nullptr;
}

// Scene queries
BulletPhysicWorld::raycast_result BulletPhysicWorld::raycast(const Vec3& starting_point, const Vec3& direction, float max_distance) {
    const Vec3 destination = starting_point + direction * max_distance;
    const btVector3 from{starting_point.x, starting_point.y, starting_point.z};
    const btVector3 to{destination.x, destination.y, destination.z};

    btCollisionWorld::ClosestRayResultCallback callback(from, to);
    dynamic_world->rayTest(from, to, callback);
}

BulletPhysicWorld::raycast_result BulletPhysicWorld::raycast(const Vec3& starting_point, const Vec3& direction, float max_distance, uint32_t collision_mask) {
    const Vec3 destination = starting_point + direction * max_distance;
    const btVector3 from{starting_point.x, starting_point.y, starting_point.z};
    const btVector3 to{destination.x, destination.y, destination.z};

    btCollisionWorld::ClosestRayResultCallback callback(from, to);
    dynamic_world->rayTest(from, to, callback);

    const btRigidBody* body = btRigidBody::upcast(callback.m_collisionObject);

    return raycast_result(BulletRigidBody{const_cast<btRigidBody*>(body)},
                          Vec3(callback.m_hitPointWorld.getX(), callback.m_hitPointWorld.getY(), callback.m_hitPointWorld.getZ()),
                          Vec3(callback.m_hitNormalWorld.getX(), callback.m_hitNormalWorld.getY(), callback.m_hitNormalWorld.getZ()),
                          (callback.m_hitPointWorld - from).length());
}

void BulletPhysicWorld::draw_debug() const {
    dynamic_world->debugDrawWorld();
}

}