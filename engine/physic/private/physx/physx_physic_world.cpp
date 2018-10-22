#include "physx_physic_world.hpp"
#include "physx_foundation.hpp"
#include "physx_physics.h"

#include <PxPhysics.h>
#include <PxPhysicsVersion.h>
#include <PxSceneDesc.h>
#include <PxScene.h>
#include <common/PxTolerancesScale.h>
#include <common/PxRenderBuffer.h>
#include <debug_draw_service.hpp>
#include <renderer_interface.hpp>
#include <PxRigidDynamic.h>
#include <PxRigidStatic.h>
#include <PxShape.h>
#include <PxMaterial.h>
#include <extensions/PxRigidBodyExt.h>
#include <PxPhysicsAPI.h>

using namespace physx;

namespace sushi {

struct PhysXPhysicWorld::impl {
    PhysXFoundation foundation;
    PhysXPhysics physics;
    PxScene* scene;
    PxMaterial* material;

    impl()
    : foundation{}
    , physics{foundation.make_physics()}
    , scene{physics.make_scene()}{
        material = physics.get().createMaterial(0.5f, 0.5f, 0.6f);
    }

    ~impl() {
        material->release();
        scene->release();
    }
};

PhysXPhysicWorld::PhysXPhysicWorld()
: pimpl(std::make_unique<PhysXPhysicWorld::impl>()) {

    pimpl->scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
    pimpl->scene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 2.0f);
    pimpl->scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.f);
}

PhysXPhysicWorld::~PhysXPhysicWorld() = default;

void PhysXPhysicWorld::step_simulation(float dt_ms) {
    pimpl->scene->simulate(dt_ms);

    pimpl->scene->fetchResults(true);
}

static Vec3 to_vec3(const PxVec3& v) {
    return {v.x, v.y, v.z};
}

static PxVec3 to_px_vec3(const Vec3& v) {
    return PxVec3(v.x, v.y, v.z);
}

static Quaternion to_quat(const PxQuat& quat) {
    return Quaternion(quat.x, quat.y, quat.z, quat.w);
}

static PxQuat to_px_quat(const Quaternion& quat) {
    return PxQuat(quat.x, quat.y, quat.z, quat.w);
}

static RGBColor to_rgb(const PxU32 v) {
    const uint8_t* raw_components = reinterpret_cast<const uint8_t*>(&v);

    return RGBColor(raw_components[0], raw_components[1], raw_components[2]);
}

static RGBAColor to_rgba(const PxU32 v) {
    const uint8_t* raw_components = reinterpret_cast<const uint8_t*>(&v);

    return RGBAColor(raw_components[0], raw_components[1], raw_components[2], raw_components[3]);
}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass) {
    PxShape* px_shape = pimpl->physics.get().createShape(PxSphereGeometry(shape.radius), *pimpl->material);
    PxTransform px_transform(to_px_vec3(transform.translation), to_px_quat(transform.rotation));
    PxRigidActor* body = nullptr;
    if(mass == 0.f) {
        // Static
        body = pimpl->physics.get().createRigidStatic(px_transform);
    }
    else {
        // Rigid
        PxRigidDynamic* dynamic = pimpl->physics.get().createRigidDynamic(px_transform);
        PxRigidBodyExt::updateMassAndInertia(*dynamic, mass);

        body = dynamic;
    }
    body->attachShape(*px_shape);
    px_shape->release();

    pimpl->scene->addActor(*body);

    return PhysXRigidBody(body);
}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass) {
    PxShape* px_shape = pimpl->physics.get().createShape(PxBoxGeometry(shape.width / 2.f, shape.height / 2.f, shape.depth / 2.f), *pimpl->material);
    PxTransform px_transform(to_px_vec3(transform.translation), to_px_quat(transform.rotation));
    PxRigidActor* body = nullptr;
    if(mass == 0.f) {
        // Static
        body = pimpl->physics.get().createRigidStatic(px_transform);
    }
    else {
        // Rigid
        PxRigidDynamic* dynamic = pimpl->physics.get().createRigidDynamic(px_transform);
        PxRigidBodyExt::updateMassAndInertia(*dynamic, mass);

        body = dynamic;
    }
    body->attachShape(*px_shape);
    px_shape->release();

    pimpl->scene->addActor(*body);

    return PhysXRigidBody(body);
}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass) {
    PxShape* px_shape = pimpl->physics.get().createShape(PxCapsuleGeometry(shape.radius, shape.height / 2.f), *pimpl->material);
    PxTransform px_transform(to_px_vec3(transform.translation), to_px_quat(transform.rotation));
    PxRigidActor* body = nullptr;
    if(mass == 0.f) {
        // Static
        body = pimpl->physics.get().createRigidStatic(px_transform);
    }
    else {
        // Rigid
        PxRigidDynamic* dynamic = pimpl->physics.get().createRigidDynamic(px_transform);
        PxRigidBodyExt::updateMassAndInertia(*dynamic, mass);

        body = dynamic;
    }
    body->attachShape(*px_shape);
    px_shape->release();

    pimpl->scene->addActor(*body);

    return PhysXRigidBody(body);
}

PhysXPhysicWorld::rigid_body_type PhysXPhysicWorld::make_rigid_body(PhysicTransform transform, PhysicPlaneShape shape, float mass) {
    PxShape* px_shape = pimpl->physics.get().createShape(PxPlaneGeometry(), *pimpl->material);
    PxTransform plane_transform = PxTransformFromPlaneEquation(PxPlane(shape.normal.x, shape.normal.y, shape.normal.z, shape.distance));
    PxTransform px_transform(to_px_vec3(transform.translation), to_px_quat(transform.rotation));
    PxRigidActor* body = nullptr;
    if(mass == 0.f) {
        // Static
        body = pimpl->physics.get().createRigidStatic(px_transform * plane_transform);
    }
    else {
        // Rigid
        PxRigidDynamic* dynamic = pimpl->physics.get().createRigidDynamic(px_transform * plane_transform);
        PxRigidBodyExt::updateMassAndInertia(*dynamic, mass);

        body = dynamic;
    }
    body->attachShape(*px_shape);
    px_shape->release();

    pimpl->scene->addActor(*body);

    return PhysXRigidBody(body);
}

PhysXPhysicWorld::joint_type PhysXPhysicWorld::join(rigid_body_type a, rigid_body_type b, PhysicRopeJoint joint) {
    PxDistanceJoint* px_joint = PxDistanceJointCreate(pimpl->physics.get(),
                                                      a.rigid_body, PxTransform(PxVec3(0.f, 0.f, 0.f)),
                                                      b.rigid_body, PxTransform(PxVec3(0.f, 0.f, 0.f)));

    px_joint->setMaxDistance(joint.length);

    return joint_type{px_joint};
}

void PhysXPhysicWorld::destroy(rigid_body_type& body) {
    pimpl->scene->removeActor(*body.rigid_body);
    body.rigid_body = nullptr;
}

void PhysXPhysicWorld::destroy(joint_type& join) {
    join.joint->release();
    join.joint = nullptr;
}

void PhysXPhysicWorld::draw_debug() const {
    const PxRenderBuffer& rb = pimpl->scene->getRenderBuffer();
    for(PxU32 i=0; i < rb.getNbLines(); ++i)
    {
        const PxDebugLine& line = rb.getLines()[i];
        DebugRendererService::get().add_line(to_vec3(line.pos0), to_vec3(line.pos1), to_rgb(line.color0));
    }

    for(PxU32 i = 0; i < rb.getNbTriangles(); ++i) {
        const PxDebugTriangle& triangle = rb.getTriangles()[i];
        DebugRendererService::get().add_triangle(to_vec3(triangle.pos0), to_vec3(triangle.pos1), to_vec3(triangle.pos2), to_rgb(triangle.color0));
    }
}

}