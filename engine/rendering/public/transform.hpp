#ifndef SPOOKYSUSHI_TRANSFORM_HPP
#define SPOOKYSUSHI_TRANSFORM_HPP

#include "types.hpp"

namespace sushi {

class Transform {
    Vec3 translation_;
    Quaternion rotation_;
    Vec3 scale_;
    mutable Mat4x4 cached_transform;
    mutable bool is_dirty;

public:
    Transform() noexcept;
    Transform(Vec3 translation, Quaternion quat) noexcept;

    Vec3 up() const noexcept;
    Vec3 forward() const noexcept;
    Vec3 right() const noexcept;

    void set_translation(const Vec3& new_translation) noexcept;
    void set_rotation(const Quaternion& new_rotation) noexcept;
    void set_scale(const Vec3& new_scale) noexcept;

    const Vec3& translation() const noexcept;
    const Quaternion& rotation() const noexcept;
    const Vec3& scale() const noexcept;

    Transform& translate(const Vec3& translation) noexcept;

    Transform& rotate(float angle, const Vec3& axis) noexcept;
    Transform& rotate(float pitch, float yaw, float roll) noexcept;
    Transform& rotate(const Quaternion& quaternion) noexcept;

    Transform& scale(const Vec3& scale) noexcept;
    Transform& scale(float uniform_scale) noexcept;

    Transform& look_at(const Vec3& target) noexcept;

    Transform& reset() noexcept;

    const Mat4x4& matrix() const noexcept;

    // Apply the parent transform to the child transform
    friend Transform operator*(const Transform& parent, const Transform& child) noexcept;
};

}

#endif
