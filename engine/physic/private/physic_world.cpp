#include "physic_world.hpp"

#include <btBulletDynamicsCommon.h>

namespace sushi {

class Bullet3DWorld : public PhysicWorld {
    btDefaultCollisionConfiguration* collision_configurations;
    btCollisionDispatcher* collision_dispatcher;
    btBroadphaseInterface* overlapping_pair_cache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamic_world;

public:
    Bullet3DWorld()
    : collision_configurations{new btDefaultCollisionConfiguration{}}
    , collision_dispatcher{new btCollisionDispatcher{collision_configurations}}
    , overlapping_pair_cache{new btDbvtBroadphase{}}
    , solver{new btSequentialImpulseConstraintSolver{}}
    , dynamic_world{new btDiscreteDynamicsWorld{collision_dispatcher, overlapping_pair_cache, solver, collision_configurations}} {
        dynamic_world->setGravity(btVector3{0.f, -9.8f, 0.f});
    }

    Bullet3DWorld(const Bullet3DWorld&) = delete;
    Bullet3DWorld& operator=(const Bullet3DWorld&) = delete;

    virtual ~Bullet3DWorld() {
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
};

std::unique_ptr<PhysicWorld> make_physic_world() {
    return std::make_unique<Bullet3DWorld>();
}

}