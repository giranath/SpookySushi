#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include <iostream>
#include <cassert>

namespace sushi {

Mat4x4 Camera::projection() const noexcept {
    return glm::perspective(70.f, 800.f / 600.f, 0.1f, 1000.f);
}

Mat4x4 Camera::view() const noexcept {
    return glm::lookAt(local_transform.translation(),
                       local_transform.translation() + local_transform.forward(),
                       Vec3{0.f, 1.f, 0.f});
}

Ray Camera::ray_from_viewport_coord(Vec2 coord) const noexcept {
    assert(coord.x <= 1.f);
    assert(coord.y <= 1.f);
    assert(coord.x >= -1.f);
    assert(coord.y >= -1.f);

    const Vec4 ray_clip(coord.x, coord.y, -1.f, 1.f);
    Vec4 ray_eye = glm::inverse(projection()) * ray_clip;
    ray_eye.z = -1.f;
    ray_eye.w = 0.f;

    const Vec3 ray_world = glm::normalize((glm::inverse(view()) * ray_eye));

    return Ray(local_transform.translation(), ray_world);
}

}
