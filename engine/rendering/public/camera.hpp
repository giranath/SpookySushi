#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "transform.hpp"
#include "types.hpp"

namespace sushi {

// TODO: Set projection with composition

class Camera {
    Vec4 ray_eye(Vec4 ray_clip) const noexcept;
    Vec4 ray_clip(Vec2 coord) const noexcept;
public:
    Transform local_transform;

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;

    Ray ray_from_viewport_coord(Vec2 coord) const noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
