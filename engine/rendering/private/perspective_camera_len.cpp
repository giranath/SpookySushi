#include "camera_len.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace sushi {

PerspectiveCameraLen::PerspectiveCameraLen(float fovy, float aspect, float near, float far)
: fovy_{fovy}
, aspect_{aspect}
, near_{near}
, far_{far}
, dirty_{true} {

}

void PerspectiveCameraLen::set_aspect(float width, float height) noexcept {
    set_aspect(width / height);
}

void PerspectiveCameraLen::set_aspect(float aspect) noexcept {
    aspect_ = aspect;
    dirty_ = true;
}

void PerspectiveCameraLen::set_planes(float near, float far) noexcept {
    set_near(near);
    set_far(far);
}

void PerspectiveCameraLen::set_near(float near) noexcept {
    near_ = near;
    dirty_ = true;
}

void PerspectiveCameraLen::set_far(float far) noexcept {
    far_ = far;
    dirty_ = true;
}

void PerspectiveCameraLen::set_fov(float fov) noexcept {
    fovy_ = fov;
    dirty_ = true;
}

Mat4x4 PerspectiveCameraLen::projection() const {
    // Cache projection
    if(dirty_) {
        projection_ = glm::perspective(fovy_, aspect_, near_, far_);
        dirty_ = false;
    }

    return projection_;
}

InfinitePerspectiveCameraLen::InfinitePerspectiveCameraLen(float fovy, float aspect, float near)
: fovy_{fovy}
, aspect_{aspect}
, near_{near}
, dirty_{true} {

}

void InfinitePerspectiveCameraLen::set_aspect(float width, float height) noexcept {
    set_aspect(width / height);
}

void InfinitePerspectiveCameraLen::set_aspect(float aspect) noexcept {
    aspect_ = aspect;
    dirty_ = true;
}

void InfinitePerspectiveCameraLen::set_near(float near) noexcept {
    near_ = near;
    dirty_ = true;
}

void InfinitePerspectiveCameraLen::set_fov(float fov) noexcept {
    fovy_ = fov;
    dirty_ = true;
}

Mat4x4 InfinitePerspectiveCameraLen::projection() const {
    if(dirty_) {
        const float tanHalfFovy = glm::tan(fovy_ / 2.f);

        // Matrix took from http://www.terathon.com/gdc07_lengyel.pdf
        projection_ = Mat4x4{0.f};
        projection_[0][0] = 1.f / (aspect_ * tanHalfFovy);
        projection_[1][1] = 1.f / tanHalfFovy;
        projection_[2][2] = glm::epsilon<float>() - 1.f;
        projection_[2][3] = -1.f;
        projection_[3][2] = (glm::epsilon<float>() - 2.f) * near_;

        dirty_ = false;
    }

    return projection_;
}

}