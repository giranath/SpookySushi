#ifndef SPOOKYSUSHI_PHYSIC_WORLD_HPP
#define SPOOKYSUSHI_PHYSIC_WORLD_HPP

#include "physic_backend.hpp"

// TODO: Import good physic world implementation
#if defined(SUSHI_PHYSIC_BACKEND_BULLET)
#include "bullet_physic_world.hpp"
#elif defined(SUSHI_PHYSIC_BACKEND_PHYSX)
#else
#error "You must specify a physic backend"
#endif

#include "physic_world_traits.hpp"

#include <type_traits>

namespace sushi {

using PhysicWorld = BulletPhysicWorld;
using RigidBody = physic_world_traits<PhysicWorld>::rigid_body_type;

//static_assert(!std::is_same_v<physic_world_traits<PhysicWorld>::rigid_body_type, void>, "invalid physic world type");
static_assert(is_rigid_body<physic_world_traits<PhysicWorld>::rigid_body_type>::value, "physic world does not support rigid bodies");

template<typename PhysicWorldType>
std::enable_if_t<physic_world_traits<PhysicWorldType>::can_draw_debug> draw_physic_debug(const PhysicWorldType& world) {
    world.draw_debug();
}

template<typename PhysicWorldType>
std::enable_if_t<!(physic_world_traits<PhysicWorldType>::can_draw_debug)> draw_physic_debug(const PhysicWorldType& world) {
    // NO OP
}

}

#endif
