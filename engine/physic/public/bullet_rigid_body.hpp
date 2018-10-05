#ifndef SPOOKYSUSHI_BULLET_RIGID_BODY_HPP
#define SPOOKYSUSHI_BULLET_RIGID_BODY_HPP

#include "rigid_body_traits.hpp"

class btRigidBody;

namespace sushi {

class BulletPhysicWorld;

class BulletRigidBody {
    friend BulletPhysicWorld;

    btRigidBody* body;

    BulletRigidBody(btRigidBody* body) noexcept;
public:
    BulletRigidBody() noexcept;

    explicit operator bool() const noexcept;
};

template<>
struct is_rigid_body<BulletRigidBody> {
    static const bool value = true;
};

}

#endif
