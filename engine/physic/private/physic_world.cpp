#include "physic_world.hpp"

#include <btBulletDynamicsCommon.h>
#include <debug_draw_service.hpp>
#include <renderer_interface.hpp>
#include <log_service.hpp>

namespace sushi {

Vec3 to_vec3(const btVector3& bt_vector3) noexcept {
    return Vec3{bt_vector3.x(), bt_vector3.y(), bt_vector3.z()};
}

RGBColor to_rgb(const btVector3& bt_color) noexcept {
    return RGBColor(bt_color.x(), bt_color.y(), bt_color.z());
}

class BulletDebugDrawer : public btIDebugDraw {
    int debug_mode;
public:
    BulletDebugDrawer()
    : debug_mode{} {

    }

    void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override {
        DebugRendererService::get().add_line(to_vec3(from), to_vec3(to), to_rgb(color));
    }

    void drawSphere (const btVector3& p, btScalar radius, const btVector3& color) override {
        DebugRendererService::get().add_sphere(to_vec3(p), radius, to_rgb(color));
    }

    void drawAabb(const btVector3& from, const btVector3& to,const btVector3& color) override {
        const float width = std::abs(to.x() - from.x());
        const float height = std::abs(to.y() - from.y());
        const float depth = std::abs(to.z() - from.z());

        const float half_width = width / 2.f;
        const float half_height = height / 2.f;
        const float half_depth = depth / 2.f;

        const Vec3 extend{half_width, half_height, half_depth};

        DebugRendererService::get().add_aabb(to_vec3(from) + extend, extend, to_rgb(color));
    }

    void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override {

    }

    void reportErrorWarning(const char* warningString) override {
        log_warning("sushi.physic.bullet3", warningString);
    }

    void draw3dText(const btVector3& location, const char* textString) override {
        // TODO: Draw 3d text
    }

    void setDebugMode(int debugMode) override {
        debug_mode = debugMode;
    }

    int getDebugMode() const override {
        return debug_mode;
    }
};

class Bullet3DWorld : public PhysicWorld {
    btDefaultCollisionConfiguration* collision_configurations;
    btCollisionDispatcher* collision_dispatcher;
    btBroadphaseInterface* overlapping_pair_cache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamic_world;
    btIDebugDraw* debug_draw;

public:
    Bullet3DWorld()
    : collision_configurations{new btDefaultCollisionConfiguration{}}
    , collision_dispatcher{new btCollisionDispatcher{collision_configurations}}
    , overlapping_pair_cache{new btDbvtBroadphase{}}
    , solver{new btSequentialImpulseConstraintSolver{}}
    , dynamic_world{new btDiscreteDynamicsWorld{collision_dispatcher, overlapping_pair_cache, solver, collision_configurations}}
    , debug_draw{new BulletDebugDrawer()}{
        dynamic_world->setGravity(btVector3{0.f, -9.8f, 0.f});

        debug_draw->setDebugMode(btIDebugDraw::DBG_DrawWireframe | btIDebugDraw::DBG_DrawAabb);
        dynamic_world->setDebugDrawer(debug_draw);
    }

    Bullet3DWorld(const Bullet3DWorld&) = delete;
    Bullet3DWorld& operator=(const Bullet3DWorld&) = delete;

    virtual ~Bullet3DWorld() {
        delete debug_draw;
        delete dynamic_world;
        delete solver;
        delete overlapping_pair_cache;
        delete collision_dispatcher;
        delete collision_configurations;
    }

    void set_gravity(const Vec3& gravity) override {
        dynamic_world->setGravity(btVector3{gravity.x, gravity.y, gravity.z});
    }

    void step_simulation(float dt) override {
        dynamic_world->stepSimulation(dt);
    }

    void draw_debug_informations() override {
        dynamic_world->debugDrawWorld();
    }
};

std::unique_ptr<PhysicWorld> make_physic_world() {
    return std::make_unique<Bullet3DWorld>();
}

}