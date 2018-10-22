#ifndef SPOOKYSUSHI_CAMERA_CONTROLLER_HPP
#define SPOOKYSUSHI_CAMERA_CONTROLLER_HPP

#include <camera.hpp>
#include <base_game.hpp>
#include <transform.hpp>

class CameraController {
public:
    using Camera = sushi::Camera;
    using Transform = sushi::Transform;
private:
    Camera* camera;
    Transform* to_follow;
    bool should_update_offset;
    sushi::Vec3 offset;

    void update_offset();

public:
    CameraController(Camera* camera = nullptr);

    void set_camera(Camera* camera) noexcept;
    void set_target(Transform* target) noexcept;

    void update(sushi::frame_duration last_frame);
};


#endif //SPOOKYSUSHI_CAMERA_CONTROLLER_HPP
