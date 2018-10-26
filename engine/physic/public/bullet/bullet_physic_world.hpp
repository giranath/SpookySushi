#ifndef SPOOKYSUSHI_BULLET_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_BULLET_PHYSIC_WORLD_HPP

#include "physic_world_traits.hpp"
#include "physic_shape.hpp"
#include "physic_transform.hpp"
#include "physic_joints.hpp"
#include "raycast_result.hpp"

#include "bullet_rigid_body.hpp"
#include "bullet_joint.hpp"
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
    using joint_type = BulletJoint;
    using raycast_result = RayCastResult<rigid_body_type>;

    BulletPhysicWorld();
    ~BulletPhysicWorld();

    void step_simulation(float dt_ms);

    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicSphereShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicBoxShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicCapsuleShape shape, float mass = 0.f);
    rigid_body_type make_rigid_body(PhysicTransform transform, PhysicPlaneShape shape);

    // Joints
    joint_type join(rigid_body_type a, rigid_body_type b, PhysicRopeJoint joint);

    void destroy(rigid_body_type& body);
    void destroy(joint_type& join);

    // Scene queries
    raycast_result raycast(const Vec3& starting_point, const Vec3& direction, float max_distance);
    raycast_result raycast(const Vec3& starting_point, const Vec3& direction, float max_distance, uint32_t collision_mask);

    void draw_debug() const;
};

template<>
struct physic_world_traits<BulletPhysicWorld> {
    using value_type = BulletPhysicWorld;
    using rigid_body_type = BulletPhysicWorld::rigid_body_type;
    using joint_type = BulletPhysicWorld::joint_type;

    static const bool can_draw_debug = true;
};

}

#endif
