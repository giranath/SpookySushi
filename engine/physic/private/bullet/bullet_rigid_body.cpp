#include "bullet_rigid_body.hpp"
#include <types.hpp>

#include <btBulletDynamicsCommon.h>

namespace sushi {

BulletRigidBody::BulletRigidBody() noexcept
: body{nullptr} {

}

BulletRigidBody::BulletRigidBody(btRigidBody* body) noexcept
: body{body} {

}

BulletRigidBody::operator bool() const noexcept {
    return body;
}

void BulletRigidBody::set_linear_damping(float damp) {
    body->setDamping(damp, body->getAngularDamping());
}

void BulletRigidBody::set_angular_damping(float damp) {
    body->setDamping(body->getLinearDamping(), damp);
}

void BulletRigidBody::apply_force_at(const Vec3& position, const Vec3& force) {
    body->applyForce(btVector3{force.x, force.y, force.z}, btVector3{position.x, position.y, position.z});
}

PhysicTransform BulletRigidBody::transform() const {
    const btTransform& world_transform = body->getWorldTransform();
    const btVector3& world_origin = world_transform.getOrigin();
    const btQuaternion& world_rotation = world_transform.getRotation();

    return PhysicTransform{Vec3{world_origin.getX(), world_origin.getY(), world_origin.getZ()},
                           Quaternion{world_rotation.getX(), world_rotation.getY(), world_rotation.getZ(), world_rotation.getW()}};
}

void BulletRigidBody::set_query_filter_mask(uint32_t filter_mask) {

}

uint32_t BulletRigidBody::query_filter_mask() const {
    return 0;
}

void BulletRigidBody::set_movement_lock(MovementLock lock) {
    btVector3 linear_constraints(0.f, 0.f, 0.f);

    if(!lock.is_locked(TranslationLock::XAxis)) {
        linear_constraints += btVector3{1.f, 0.f, 0.f};
    }

    if(!lock.is_locked(TranslationLock::YAxis)) {
        linear_constraints += btVector3{0.f, 1.f, 0.f};
    }

    if(!lock.is_locked(TranslationLock::ZAxis)) {
        linear_constraints += btVector3{0.f, 0.f, 1.f};
    }

    body->setLinearFactor(linear_constraints);

    btVector3 angular_constraints(0.f, 0.f, 0.f);

    if(!lock.is_locked(RotationLock::Pitch)) {
        angular_constraints += btVector3{1.f, 0.f, 0.f};
    }

    if(!lock.is_locked(RotationLock::Yaw)) {
        angular_constraints += btVector3{0.f, 1.f, 0.f};
    }

    if(!lock.is_locked(RotationLock::Roll)) {
        angular_constraints += btVector3{0.f, 0.f, 1.f};
    }

    body->setLinearFactor(linear_constraints);
}


}