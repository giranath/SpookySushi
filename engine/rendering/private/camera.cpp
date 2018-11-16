#include "camera.hpp"
#include "camera_len.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <cassert>

namespace sushi {

Camera::Camera(std::shared_ptr<CameraLen> len)
: len_{std::move(len)} {
    if(len_ == nullptr) throw MissingLen{};
}

Camera::Camera(std::unique_ptr<CameraLen> len)
: len_{std::move(len)}{
    if(len_ == nullptr) throw MissingLen{};
}

Mat4x4 Camera::projection() const noexcept {
    return len_->projection();
}

Mat4x4 Camera::view() const noexcept {
    return glm::lookAt(local_transform.translation(),
                       local_transform.translation() + local_transform.forward(),
                       local_transform.up());
}

Vec4 Camera::ray_eye(Vec4 ray_clip) const noexcept {
    Vec4 ray_eye = glm::inverse(projection()) * ray_clip;
    ray_eye.z = -1.f;
    ray_eye.w = 0.f;

    return ray_eye;
}

Vec4 Camera::ray_clip(Vec2 coord) const noexcept {
    return Vec4{coord.x, coord.y, -1.f, 1.f};
}

Ray Camera::ray_from_viewport_coord(Vec2 coord) const noexcept {
    assert(coord.x <= 1.f);
    assert(coord.y <= 1.f);
    assert(coord.x >= -1.f);
    assert(coord.y >= -1.f);

    return Ray{local_transform.translation(), glm::normalize((glm::inverse(view()) * ray_eye(ray_clip(coord))))};
}

}
