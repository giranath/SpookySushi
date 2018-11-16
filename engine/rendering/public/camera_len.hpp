#ifndef SPOOKYSUSHI_CAMERA_LEN_HPP
#define SPOOKYSUSHI_CAMERA_LEN_HPP

#include "types.hpp"

namespace sushi {

struct CameraLen {
    virtual ~CameraLen() = default;
    virtual Mat4x4 projection() const = 0;
};

struct InvalidPlanes{};

class PerspectiveCameraLen : public CameraLen {
    float fovy_;
    float aspect_;
    float near_;
    float far_;

    mutable bool dirty_;
    mutable Mat4x4 projection_;

public:
    PerspectiveCameraLen(float fovy, float aspect, float near, float far);

    void set_aspect(float width, float height) noexcept;
    void set_aspect(float aspect) noexcept;
    void set_planes(float near, float far) noexcept;
    void set_near(float near) noexcept;
    void set_far(float far) noexcept;
    void set_fov(float fov) noexcept;
    Mat4x4 projection() const override;
};

class InfinitePerspectiveCameraLen : public CameraLen {
    float fovy_;
    float aspect_;
    float near_;

    mutable bool dirty_;
    mutable Mat4x4 projection_;

public:
    InfinitePerspectiveCameraLen(float fovy, float aspect, float near);

    void set_aspect(float width, float height) noexcept;
    void set_aspect(float aspect) noexcept;
    void set_near(float near) noexcept;
    void set_fov(float fov) noexcept;
    Mat4x4 projection() const override;
};

}

#endif //SPOOKYSUSHI_CAMERA_LEN_HPP
