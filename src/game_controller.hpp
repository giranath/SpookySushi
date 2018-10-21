#ifndef SPOOKYSUSHI_GAME_CONTROLLER_HPP
#define SPOOKYSUSHI_GAME_CONTROLLER_HPP

#include <input_processor.hpp>

#include <memory>

class GameController {
    std::unique_ptr<sushi::AxisInputProcessor> move_forward_processor;
    std::unique_ptr<sushi::AxisInputProcessor> move_strate_processor;
    std::unique_ptr<sushi::AxisInputProcessor> yaw_camera_processor;
    std::unique_ptr<sushi::AxisInputProcessor> pitch_camera_processor;
    std::unique_ptr<sushi::StateInputProcessor> apply_boost;
public:
    GameController();

    void register_inputs();
    void unregister_inputs();

    float get_move_forward_value() const noexcept;
    float get_move_strate_value() const noexcept;
    float get_camera_yaw_value() const noexcept;
    float get_camera_pitch_value() const noexcept;
    bool should_apply_boost() const noexcept;
};


#endif //SPOOKYSUSHI_GAME_CONTROLLER_HPP
