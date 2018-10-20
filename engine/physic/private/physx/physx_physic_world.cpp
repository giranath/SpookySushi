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

using namespace physx;

namespace sushi {

struct PhysXPhysicWorld::impl {
    PhysXFoundation foundation;
    PhysXPhysics physics;
    PxScene* scene;

    impl()
    : foundation{}
    , physics{foundation.make_physics()}
    , scene{physics.make_scene()}{
    }

    ~impl() {
        scene->release();
    }
};

PhysXPhysicWorld::PhysXPhysicWorld()
: pimpl(std::make_unique<PhysXPhysicWorld::impl>()) {

    pimpl->scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
}

PhysXPhysicWorld::~PhysXPhysicWorld() = default;

void PhysXPhysicWorld::step_simulation(float dt_ms) {
    pimpl->scene->simulate(dt_ms);

    pimpl->scene->fetchResults(true);
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

static Vec3 to_vec3(const PxVec3& v) {
    return {v.x, v.y, v.z};
}

static RGBColor to_rgb(const PxU32 v) {
    const uint8_t* raw_components = reinterpret_cast<const uint8_t*>(&v);

    return RGBColor(raw_components[0], raw_components[1], raw_components[2]);
}

static RGBAColor to_rgba(const PxU32 v) {
    const uint8_t* raw_components = reinterpret_cast<const uint8_t*>(&v);

    return RGBAColor(raw_components[0], raw_components[1], raw_components[2], raw_components[3]);
}

void PhysXPhysicWorld::draw_debug() const {
    const PxRenderBuffer& rb = pimpl->scene->getRenderBuffer();
    for(PxU32 i=0; i < rb.getNbLines(); i++)
    {
        const PxDebugLine& line = rb.getLines()[i];
        DebugRendererService::get().add_line(to_vec3(line.pos0), to_vec3(line.pos1), to_rgb(line.color0));
    }
}

}