#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "transform.hpp"
#include "types.hpp"

#include <memory>

namespace sushi {

struct CameraLen;

class Camera {
    Vec4 ray_eye(Vec4 ray_clip) const noexcept;
    Vec4 ray_clip(Vec2 coord) const noexcept;
public:
    struct MissingLen {};

private:
    std::shared_ptr<CameraLen> len_;
public:
    Transform local_transform;

    // Construct a camera with a len
    explicit Camera(std::shared_ptr<CameraLen> len);
    explicit Camera(std::unique_ptr<CameraLen> len);

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;

    Ray ray_from_viewport_coord(Vec2 coord) const noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
