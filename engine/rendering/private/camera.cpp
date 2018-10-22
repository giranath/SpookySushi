#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <glm/gtx/quaternion.hpp>

namespace sushi {

Mat4x4 Camera::projection() const noexcept {
    return glm::perspective(70.f, 800.f / 600.f, 0.1f, 1000.f);
}

Mat4x4 Camera::view() const noexcept {
    const Mat4x4 translation = glm::inverse(glm::translate(glm::mat4{1.f}, local_transform.translation()));
    const Mat4x4 rotation = glm::toMat4(local_transform.rotation());

    return rotation * translation;
}

}
