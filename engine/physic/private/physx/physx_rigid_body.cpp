#include "physx_rigid_body.hpp"
#include <PxRigidBody.h>
#include <PxRigidDynamic.h>
#include <extensions/PxRigidBodyExt.h>
#include <log_service.hpp>

using namespace physx;

namespace sushi {

PhysXRigidBody::PhysXRigidBody() noexcept
: rigid_body{nullptr} {

}

PhysXRigidBody::PhysXRigidBody(physx::PxRigidActor* rigid_body) noexcept
: rigid_body{rigid_body} {

}

void PhysXRigidBody::set_linear_damping(float damp) {
    if(good() && rigid_body->is<PxRigidDynamic>()) {
        PxRigidDynamic* dynamic = rigid_body->is<PxRigidDynamic>();

        dynamic->setLinearDamping(damp);
    }
    else {
        throw RigidBodyNotBound{};
    }
}

void PhysXRigidBody::set_angular_damping(float damp) {
    if(good() && rigid_body->is<PxRigidDynamic>()) {
        PxRigidDynamic* dynamic = rigid_body->is<PxRigidDynamic>();

        dynamic->setAngularDamping(damp);
    }
    else {
        throw RigidBodyNotBound{};
    }
}

void PhysXRigidBody::apply_force_at(const Vec3& position, const Vec3& force) {
    if(good() && rigid_body->is<PxRigidDynamic>()) {
        PxRigidDynamic* dynamic = rigid_body->is<PxRigidDynamic>();

        PxRigidBodyExt::addForceAtLocalPos(*dynamic, PxVec3(force.x, force.y, force.z), PxVec3(position.x, position.y, position.z));
    }
    else {
        throw RigidBodyNotBound{};
    }
}

PhysicTransform PhysXRigidBody::transform() const {
    if (good()) {
        PxTransform current_global_pose = rigid_body->getGlobalPose();

        return PhysicTransform(Vec3(current_global_pose.p.x, current_global_pose.p.y, current_global_pose.p.z),
                               Quaternion(current_global_pose.q.x, current_global_pose.q.y, current_global_pose.q.z,
                                          current_global_pose.q.w));
    }
    else {
        throw RigidBodyNotBound{};
    }

    return PhysicTransform();
}

void PhysXRigidBody::set_query_filter_mask(uint32_t filter_mask) {
    if(!good()) throw RigidBodyNotBound{};

    std::vector<PxShape*> shapes(rigid_body->getNbShapes(), nullptr);
    rigid_body->getShapes(shapes.data(), shapes.size(), 0);

    PxFilterData filter_data;
    filter_data.word0 = filter_mask;
    for(PxShape* shape : shapes) {
        shape->acquireReference();
        rigid_body->detachShape(*shape);
        shape->setQueryFilterData(filter_data);
        rigid_body->attachShape(*shape);
        shape->release();
    }
}

uint32_t PhysXRigidBody::query_filter_mask() const {
    if(!good()) throw RigidBodyNotBound{};
    if(rigid_body->getNbShapes() == 0) throw std::out_of_range("this rigid body has no shapes");

    PxShape* first_shape = nullptr;
    rigid_body->getShapes(&first_shape, 1, 0);

    return first_shape->getQueryFilterData().word0;
}

void PhysXRigidBody::set_movement_lock(MovementLock lock) {
    if(!good()) throw RigidBodyNotBound{};

    PxRigidDynamic* dynamic = rigid_body->is<PxRigidDynamic>();
    if(dynamic) {
        PxRigidDynamicLockFlags lock_flags;

        dynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, lock.is_locked(RotationLock::Yaw));
        dynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, lock.is_locked(RotationLock::Pitch));
        dynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, lock.is_locked(RotationLock::Roll));
        dynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_X, lock.is_locked(TranslationLock::XAxis));
        dynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, lock.is_locked(TranslationLock::YAxis));
        dynamic->setRigidDynamicLockFlag(PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, lock.is_locked(TranslationLock::ZAxis));
    }
}

}
