#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm/gtx/quaternion.hpp>

namespace sushi {

Mat4x4 Camera::projection() const noexcept {
    return glm::perspective(70.f, 800.f / 600.f, 0.1f, 1000.f);
}

Mat4x4 Camera::view() const noexcept {
    return glm::lookAt(local_transform.translation(),
                       local_transform.translation() + local_transform.forward(),
                       Vec3{0.f, 1.f, 0.f});
}

}
