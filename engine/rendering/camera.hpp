#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "../core/types.hpp"

namespace sushi {

class Camera {
    Vec3 local_position_;
    Quaternion local_rotation_;
public:
    Camera() noexcept;

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;
    Mat4x4 local_transform() const noexcept;

    const Vec3& local_position() const noexcept;
    Vec3& local_position() noexcept;

    const Quaternion& local_rotation() const noexcept;
    Quaternion& local_rotation() noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
