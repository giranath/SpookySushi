#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "transform.hpp"
#include "types.hpp"

namespace sushi {

struct CameraRay {
    Vec3 origin;
    Vec3 direction;

    CameraRay() = default;
    CameraRay(const Vec3& origin, const Vec3& direction)
    : origin{ origin }
    , direction{ direction } {

    }
};

struct Camera {
    Transform local_transform;

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;

    CameraRay ray_from_viewport(Vec2 coord) const noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
