#include <glm/gtc/matrix_transform.hpp>
#include "transform.hpp"

namespace sushi {

Transform::Transform() noexcept
: translation_{}
, rotation_{}
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

void Transform::set_translation(const Vec3 &new_translation) noexcept {
    translation_ = new_translation;

    is_dirty = true;
}

void Transform::set_rotation(const Quaternion &new_rotation) noexcept {
    rotation_ = new_rotation;

    is_dirty = true;
}

void Transform::set_scale(const Vec3 &new_scale) noexcept {
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
    rotation_ *= glm::angleAxis(angle, axis);

    is_dirty = true;
    return *this;
}

Transform& Transform::rotate(float pitch, float yaw, float roll) noexcept {
    rotation_ *= Quaternion{Vec3(pitch, yaw, roll)};

    is_dirty = true;
    return *this;
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

Transform& Transform::reset() noexcept {
    translation_ = Vec3{};
    rotation_ = Quaternion{};
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