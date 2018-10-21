#include "physx_rigid_body.hpp"
#include <PxRigidBody.h>
#include <PxRigidDynamic.h>
#include <extensions/PxRigidBodyExt.h>

using namespace physx;

namespace sushi {

PhysXRigidBody::PhysXRigidBody() noexcept
: rigid_body{nullptr} {

}

PhysXRigidBody::PhysXRigidBody(physx::PxRigidActor* rigid_body) noexcept
: rigid_body{rigid_body} {

}

void PhysXRigidBody::set_linear_damping(float damp) {
    if(rigid_body->is<PxRigidDynamic>()) {
        PxRigidDynamic* dynamic = rigid_body->is<PxRigidDynamic>();

        dynamic->setLinearDamping(damp);
    }
}

void PhysXRigidBody::apply_force_at(const Vec3& position, const Vec3& force) {
    if(rigid_body->is<PxRigidDynamic>()) {
        PxRigidDynamic* dynamic = rigid_body->is<PxRigidDynamic>();

        PxRigidBodyExt::addForceAtLocalPos(*dynamic, PxVec3(force.x, force.y, force.z), PxVec3(position.x, position.y, position.z));

    }
}

PhysicTransform PhysXRigidBody::transform() const noexcept {
    PxTransform current_global_pose = rigid_body->getGlobalPose();

    return PhysicTransform(Vec3(current_global_pose.p.x, current_global_pose.p.y, current_global_pose.p.z),
                           Quaternion(current_global_pose.q.x, current_global_pose.q.y, current_global_pose.q.z, current_global_pose.q.w));
}

}
