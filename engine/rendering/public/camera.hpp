#ifndef SPOOKYSUSHI_CAMERA_HPP
#define SPOOKYSUSHI_CAMERA_HPP

#include "transform.hpp"
#include "types.hpp"

namespace sushi {

struct Camera {
    Transform local_transform;

    Mat4x4 projection() const noexcept;
    Mat4x4 view() const noexcept;
};

}


#endif //SPOOKYSUSHI_CAMERA_HPP
