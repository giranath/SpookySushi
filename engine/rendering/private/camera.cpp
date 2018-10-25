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

CameraRay Camera::ray_from_viewport(Vec2 coord) const noexcept {
#if 0
    const Mat4x4 unproject = glm::inverse(projection() * view());
    const Vec4 homogene_min_mouse_coord(coord.x, coord.y, 0.f, 1.f);
    const Vec4 homogene_max_mouse_coord(coord.x, coord.y, 1.f, 1.f);

    const Vec4 in_world_min = unproject * homogene_min_mouse_coord;
    const Vec4 in_world_max = unproject * homogene_max_mouse_coord;

    const Vec3 in_world_min_3(in_world_min.x, in_world_min.y, in_world_min.z);
    const Vec3 in_world_max_3(in_world_max.x, in_world_max.y, in_world_max.z);

    const Vec3 direction = in_world_max_3 - in_world_min_3;

    return CameraRay(local_transform.translation(), glm::normalize(direction));
#endif
    const Vec3 ray_normalized_device_coordinate(coord.x, coord.y, 1.f);
    const Vec4 ray_clip(ray_normalized_device_coordinate.x, ray_normalized_device_coordinate.y, -1.f, 1.f);
    const Vec4 ray_eye = glm::inverse(projection()) * ray_clip;
    const Vec4 ray_eye_direction(ray_eye.x, ray_eye.y, 1.f, -1.f);
    const Vec4 ray_world_space_hom = glm::inverse(view()) * ray_eye_direction;
    const Vec3 ray_world_space = glm::normalize(Vec3(ray_world_space_hom.x, ray_world_space_hom.y, ray_world_space_hom.z));

    return CameraRay(local_transform.translation(), ray_world_space);
}

}
