#ifndef SPOOKYSUSHI_RAYCAST_RESULT_HPP
#define SPOOKYSUSHI_RAYCAST_RESULT_HPP

#include <types.hpp>

namespace sushi {

template<typename RigidBodyType>
class RayCastResult {
    RigidBodyType rigidbody_;
    Vec3 position_;
    Vec3 normal_;
    float distance_;
public:
    RayCastResult() = default;
    RayCastResult(RigidBodyType rigidbody, Vec3 position, Vec3 normal, float distance)
    : rigidbody_{rigidbody}
    , position_{position}
    , normal_{normal}
    , distance_{distance} {

    }

    operator bool() const noexcept {
        return static_cast<bool>(rigidbody_);
    }

    RigidBodyType rigidbody() const noexcept {
        return rigidbody_;
    }

    float distance() const noexcept {
        return distance_;
    }

    const Vec3& position() const noexcept {
        return position_;
    }

    const Vec3& normal() const noexcept {
        return normal_;
    }
};

}

#endif //SPOOKYSUSHI_RAYCAST_RESULT_HPP
