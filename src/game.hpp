#ifndef SPOOKYSUSHI_GAME_HPP
#define SPOOKYSUSHI_GAME_HPP

#include <application/base_game.hpp>
#include <rendering/camera.hpp>
#include <input/input_context.hpp>
#include <input/input_handler.hpp>

class game : public sushi::base_game {
    sushi::Camera main_camera;
    sushi::InputContext inputs;
    std::unique_ptr<sushi::AxisInputHandler> move_forward_input;
    std::unique_ptr<sushi::AxisInputHandler> move_strate_input;
public:
    void on_start() override;
    void on_frame(sushi::frame_duration last_frame) override;
    void on_late_frame(sushi::frame_duration last_frame) override;
    void on_render(sushi::ProxyRenderer renderer) override;
    void on_stop() override;
};


#endif //SPOOKYSUSHI_GAME_HPP
