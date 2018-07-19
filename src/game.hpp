#ifndef SPOOKYSUSHI_GAME_HPP
#define SPOOKYSUSHI_GAME_HPP

#include <application/public/base_game.hpp>
#include "../engine/rendering/public/camera.hpp"

class Game : public sushi::BaseGame {
    sushi::Camera main_camera;
public:
    void on_start() override;
    void on_frame(sushi::frame_duration last_frame) override;
    void on_late_frame(sushi::frame_duration last_frame) override;
    void on_render(sushi::ProxyRenderer renderer) override;
    void on_stop() override;
};


#endif //SPOOKYSUSHI_GAME_HPP
