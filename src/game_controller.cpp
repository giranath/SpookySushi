#include <axis_input_processor.hpp>
#include <input_state.hpp>
#include <action_input_processor.hpp>
#include <input_bus_service.hpp>
#include "game_controller.hpp"

GameController::GameController()
: move_forward_processor{std::make_unique<sushi::KeyAxisInputProcessor>(sushi::Key::W, sushi::Key::S)}
, move_strate_processor{ std::make_unique<sushi::KeyAxisInputProcessor>(sushi::Key::A, sushi::Key::D)}
, yaw_camera_processor{  std::make_unique<sushi::MouseAxisInputProcessor>(sushi::MouseAxis::Horizontal)}
, pitch_camera_processor{std::make_unique<sushi::MouseAxisInputProcessor>(sushi::MouseAxis::Vertical)}
, apply_boost{std::make_unique<sushi::KeyStateInputProcessor>(sushi::Key::Space)}
, shoot_left_grappling{std::make_unique<sushi::KeyActionInputProcessor>(sushi::Key::Q)}
, shoot_right_grappling{std::make_unique<sushi::KeyActionInputProcessor>(sushi::Key::E)}{
}

void GameController::register_inputs() {
    sushi::InputProcessorService::get().register_processor(move_forward_processor.get());
    sushi::InputProcessorService::get().register_processor(move_strate_processor.get());
    sushi::InputProcessorService::get().register_processor(yaw_camera_processor.get());
    sushi::InputProcessorService::get().register_processor(pitch_camera_processor.get());
    sushi::InputProcessorService::get().register_processor(apply_boost.get());
    sushi::InputProcessorService::get().register_processor(shoot_left_grappling.get());
    sushi::InputProcessorService::get().register_processor(shoot_right_grappling.get());
}

void GameController::unregister_inputs() {
    sushi::InputProcessorService::get().unregister_processor(move_forward_processor.get());
    sushi::InputProcessorService::get().unregister_processor(move_strate_processor.get());
    sushi::InputProcessorService::get().unregister_processor(yaw_camera_processor.get());
    sushi::InputProcessorService::get().unregister_processor(pitch_camera_processor.get());
    sushi::InputProcessorService::get().unregister_processor(apply_boost.get());
    sushi::InputProcessorService::get().unregister_processor(shoot_left_grappling.get());
    sushi::InputProcessorService::get().unregister_processor(shoot_right_grappling.get());
}

float GameController::get_move_forward_value() const noexcept {
    return move_forward_processor->value();
}

float GameController::get_move_strate_value() const noexcept {
    return move_strate_processor->value();
}

float GameController::get_camera_yaw_value() const noexcept {
    return yaw_camera_processor->value();
}

float GameController::get_camera_pitch_value() const noexcept {
    return pitch_camera_processor->value();
}

bool GameController::should_apply_boost() const noexcept {
    return apply_boost->is_active();
}

bool GameController::should_shoot_left_grappling() const noexcept {
    return shoot_left_grappling->state() == sushi::ActionInputProcessor::Start;
}

bool GameController::should_shoot_right_grappling() const noexcept {
    return shoot_right_grappling->state() == sushi::ActionInputProcessor::Start;
}

bool GameController::should_release_left_grappling() const noexcept {
    return shoot_left_grappling->state() == sushi::ActionInputProcessor::End;
}

bool GameController::should_release_right_grappling() const noexcept {
    return shoot_right_grappling->state() == sushi::ActionInputProcessor::End;
}