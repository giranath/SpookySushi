#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace sushi {

Camera::Camera() noexcept
: local_position_{}
, local_rotation_{} {

}

Mat4x4 Camera::projection() const noexcept {
    // TODO: Not hardcode it
    return glm::perspective(35.f, 800.f / 600.f, 0.1f, 1000.f);
}

Mat4x4 Camera::view() const noexcept {
    const glm::vec3 direction = local_rotation_ * Vec3{0.f, 0.f, 1.f};

    return glm::lookAt(local_position_, local_position_ + direction, Vec3{0.f, 1.f, 0.f});
}

Mat4x4 Camera::local_transform() const noexcept {
    const Mat4x4 translation = glm::translate(glm::mat4{1.f}, local_position_);
    const Mat4x4 rotation = glm::mat4_cast(local_rotation_);
    const Mat4x4 scale = glm::scale(glm::mat4{1.f}, glm::vec3{1.f, 1.f, 1.f});

    return translation * rotation * scale;
}

const Vec3& Camera::local_position() const noexcept {
    return local_position_;
}

Vec3& Camera::local_position() noexcept {
    return local_position_;
}

const Quaternion& Camera::local_rotation() const noexcept {
    return local_rotation_;
}

Quaternion& Camera::local_rotation() noexcept {
    return local_rotation_;
}

}