#include "bullet_rigid_body.hpp"

namespace sushi {

BulletRigidBody::BulletRigidBody() noexcept
: body{nullptr} {

}

BulletRigidBody::BulletRigidBody(btRigidBody* body) noexcept
: body{body} {

}

BulletRigidBody::operator bool() const noexcept {
    return body;
}

}