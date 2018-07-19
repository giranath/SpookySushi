#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace sushi {

void Camera::update_vectors() noexcept {
    right_ = glm::normalize(glm::cross(Vec3{0.f, 1.f, 0.f}, direction));
    up_ = glm::cross(direction, right_);
}

Camera::Camera() noexcept
: eye_position{}
, direction{0.f, 0.f, -1.f}
, up_{0.f, 1.f, 0.f}
, right_{1.f, 0.f, 0.f} {

}

Mat4x4 Camera::projection() const noexcept {
    return glm::perspective(70.f, 800.f / 600.f, 0.1f, 1000.f);
}

Mat4x4 Camera::view() const noexcept {
    return glm::lookAt(eye_position, eye_position + direction, Vec3{0.f, 1.f, 0.f});
}

void Camera::look_at(const Vec3& target) noexcept {
    direction = glm::normalize(eye_position - target);
    update_vectors();
}

const Vec3& Camera::right() const noexcept {
    return right_;
}

const Vec3& Camera::up() const noexcept {
    return up_;
}

const Vec3& Camera::forward() const noexcept {
    return direction;
}

Camera& Camera::translate(const Vec3& translation) noexcept {
    eye_position += translation;

    return *this;
}

Camera& Camera::rotate(float pitch, float yaw) noexcept {
    pitch_ += pitch;
    yaw_ += yaw;

    direction.x = std::cos(pitch_) * std::cos(yaw_);
    direction.y = std::sin(pitch_);
    direction.z = std::cos(pitch_) * std::sin(yaw_);

    direction = glm::normalize(direction);
    update_vectors();

    return *this;
}

Camera& Camera::reset() noexcept {
    eye_position = Vec3{};
    direction = Vec3{0.f, 0.f, -1.f};
    up_ = Vec3{0.f, 1.f, 0.f};
    right_ = Vec3{1.f, 0.f, 0.f};

    return *this;
}

}
