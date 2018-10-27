#include "camera_controller.hpp"

void CameraController::update_offset() {
    if(should_update_offset) {
        offset = to_follow->translation() - camera->local_transform.translation();
        should_update_offset = false;
    }
}

CameraController::CameraController(Camera* camera)
: camera{camera} {

}

void CameraController::set_camera(Camera* camera) noexcept {
    this->camera = camera;

    should_update_offset = true;
}

void CameraController::set_target(Transform* target) noexcept {
    this->to_follow = target;

    should_update_offset = true;
}

void CameraController::update(sushi::frame_duration last_frame) {
    if(camera && to_follow) {
        update_offset();

        camera->local_transform.set_translation(to_follow->translation() + offset);
        camera->local_transform.look_at(to_follow->translation());
    }
}