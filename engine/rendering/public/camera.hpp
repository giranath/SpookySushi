#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "transform.hpp"
#include "types.hpp"

namespace sushi {

struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray() noexcept = default;
    Ray(Vec3 origin, Vec3 direction) noexcept
    : origin{origin}
    , direction{direction} {

    }
};

struct Camera {
    Transform local_transform;

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;

    Ray ray_from_viewport_coord(Vec2 coord) const noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
