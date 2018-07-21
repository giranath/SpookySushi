#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

namespace sushi {

Mat4x4 Camera::projection() const noexcept {
    return glm::perspective(70.f, 800.f / 600.f, 0.1f, 1000.f);
}

Mat4x4 Camera::view() const noexcept {
    const Vec3& eye_position = local_transform.translation();
    const Vec3& direction = local_transform.forward();

    return glm::lookAt(eye_position, eye_position + direction, local_transform.up());
}

}
