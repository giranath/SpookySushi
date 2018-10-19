#ifndef SPOOKYSUSHI_BULLET_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_BULLET_PHYSIC_WORLD_HPP

#include "physic_world_traits.hpp"
#include "physic_shape.hpp"
#include "physic_transform.hpp"

#include "bullet_rigid_body.hpp"
#include <memory>

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class btIDebugDraw;

namespace sushi {

class BulletPhysicWorld {
    std::unique_ptr<btDefaultCollisionConfiguration> collision_configurations;
    std::unique_ptr<btCollisionDispatcher> collision_dispatcher;
    std::unique_ptr<btBroadphaseInterface> overlapping_pair_cache;
    std::unique_ptr<btSequentialImpulseConstraintSolver> solver;
    std::unique_ptr<btDiscreteDynamicsWorld> dynamic_world;
    std::unique_ptr<btIDebugDraw> debug_draw;
public:
    using rigid_body_type = BulletRigidBody;

    BulletPhysicWorld();
    ~BulletPhysicWorld();

    void step_simulation(float dt_ms);

    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass = 0.f);

    void draw_debug() const;
};

template<>
struct physic_world_traits<BulletPhysicWorld> {
    using value_type = BulletPhysicWorld;
    using rigid_body_type = BulletPhysicWorld::rigid_body_type;

    static const bool can_draw_debug = true;
};

}

#endif
