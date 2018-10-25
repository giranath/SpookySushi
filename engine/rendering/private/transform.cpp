#include "transform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>

#include <cmath>

namespace sushi {

Transform::Transform() noexcept
: translation_{}
, rotation_{1.f, 0.f, 0.f, 0.f}
, scale_{1.f, 1.f, 1.f}
, cached_transform{}
, is_dirty{true} {

}

Transform::Transform(Vec3 translation, Quaternion quat) noexcept
: translation_{translation}
, rotation_{quat}
, scale_{1.f, 1.f, 1.f}
, cached_transform{}
, is_dirty{true} {
}

Vec3 Transform::up() const noexcept {
    return rotation_ * Vec3{0.f, 1.f, 0.f};
}

Vec3 Transform::forward() const noexcept {
    return rotation_ * Vec3{0.f, 0.f, 1.f};
}

Vec3 Transform::right() const noexcept {
    return rotation_ * Vec3{1.f, 0.f, 0.f};
}

void Transform::set_translation(const Vec3& new_translation) noexcept {
    translation_ = new_translation;

    is_dirty = true;
}

void Transform::set_rotation(const Quaternion& new_rotation) noexcept {
    rotation_ = new_rotation;

    is_dirty = true;
}

void Transform::set_scale(const Vec3& new_scale) noexcept {
    scale_ = new_scale;

    is_dirty = true;
}

const Vec3& Transform::translation() const noexcept {
    return translation_;
}

const Quaternion& Transform::rotation() const noexcept {
    return rotation_;
}

const Vec3& Transform::scale() const noexcept {
    return scale_;
}

Transform& Transform::translate(const Vec3& translation) noexcept {
    translation_ += translation;

    is_dirty = true;
    return *this;
}

Transform& Transform::rotate(float angle, const Vec3& axis) noexcept {
    rotation_ = glm::rotate(rotation_, angle, axis);

    is_dirty = true;
    return *this;
}

Transform& Transform::rotate(float pitch, float yaw, float roll) noexcept {
    return rotate(Quaternion{Vec3{pitch, yaw, roll}});
}

Transform& Transform::rotate(const Quaternion& quaternion) noexcept {
    rotation_ *= quaternion;

    is_dirty = true;
    return *this;
}

Transform& Transform::scale(const Vec3& scale) noexcept {
    scale_ *= scale;

    is_dirty = true;
    return *this;
}

Transform& Transform::scale(float uniform_scale) noexcept {
    scale_ *= Vec3(uniform_scale, uniform_scale, uniform_scale);

    is_dirty = true;
    return *this;
}

static Quaternion direction_to_quat(const Vec3& direction) noexcept {
    const float angle = std::atan2(direction.x, direction.z);
    return Quaternion{0.f, std::sin(angle / 2.f), 0.f, std::cos(angle / 2.f)};
}

Transform& Transform::look_at(const Vec3& target) noexcept {
#if 0
    const Vec3 target_direction = glm::normalize(target - translation_);
    const Vec3 initial_direction = forward();

    const float cos_theta = glm::dot(initial_direction, target_direction);

    Vec3 rotation_axis;
    Quaternion rotation_quat;
    if(cos_theta < -1 + 0.001f) {
        rotation_axis = glm::cross(Vec3(0.f, 0.f, 1.f), initial_direction);
        if(glm::length2(rotation_axis) < 0.01f) {
            rotation_axis = glm::cross(Vec3{1.f, 0.f, 0.f}, initial_direction);
        }

        rotation_axis = glm::normalize(rotation_axis);
        rotation_quat = glm::angleAxis(glm::pi<float>(), rotation_axis);
    }
    else {
        rotation_axis = glm::cross(initial_direction, target_direction);

        const float s = glm::sqrt((1.f + cos_theta) * 2.f);
        const float inv_s = 1 / s;

        rotation_quat = Quaternion(s * 0.5f, rotation_axis.x * inv_s, rotation_axis.y * inv_s, rotation_axis.z * inv_s);
    }

    rotation_ *= rotation_quat;
#endif

    rotation_ = glm::toQuat(glm::lookAt(translation(), target, Vec3{0.f, 1.f, 0.f}));

    is_dirty = true;
    return *this;
}

Transform& Transform::reset() noexcept {
    translation_ = Vec3{};
    rotation_ = Quaternion{1.f, 0.f, 0.f, 0.f};
    scale_ = Vec3{1.f, 1.f, 1.f};
    is_dirty = true;

    return *this;
}

const Mat4x4& Transform::matrix() const noexcept {
    if(is_dirty) {
        const Mat4x4 translation = glm::translate(glm::mat4{1.f}, translation_);
        const Mat4x4 rotation = glm::mat4_cast(glm::normalize(rotation_));
        const Mat4x4 scale = glm::scale(glm::mat4{1.f}, scale_);

        cached_transform = translation * rotation * scale;//scale * rotation * translation;
        is_dirty = false;
    }

    return cached_transform;
}

Transform operator*(const Transform& parent, const Transform& child) noexcept {
    Transform result;
    result.translation_ = parent.translation_ + child.translation_;
    result.rotation_ = parent.rotation_ * child.rotation_;
    result.scale_ = parent.scale_ * child.scale_;
    result.is_dirty = true;

    return result;
}

}