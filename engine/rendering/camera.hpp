#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "transform.hpp"
#include "../core/types.hpp"

namespace sushi {

class Camera {
    Vec3 eye_position;
    Vec3 direction;
    Vec3 right_;
    Vec3 up_;
    float pitch_, yaw_;

    void update_vectors() noexcept;

public:
    Camera() noexcept;

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;

    void look_at(const Vec3& target) noexcept;
    const Vec3& right() const noexcept;
    const Vec3& up() const noexcept;
    const Vec3& forward() const noexcept;

    Camera& translate(const Vec3& translation) noexcept;
    Camera& rotate(float pitch, float yaw) noexcept;

    Camera& reset() noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
