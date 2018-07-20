#ifndef SPOOKYSUSHI_GAME_HPP
#define SPOOKYSUSHI_GAME_HPP

#include "game_controller.hpp"

#include <program.hpp>
#include <base_game.hpp>
#include <camera.hpp>
#include <static_mesh_builder.hpp>

class Game : public sushi::BaseGame {
    sushi::Camera main_camera;
    GameController controller;
    sushi::gl::program default_shader;
    sushi::StaticMeshPtr mesh;

    void prepare_shader();

public:
    void on_start() override;
    void on_frame(sushi::frame_duration last_frame) override;
    void on_late_frame(sushi::frame_duration last_frame) override;
    void on_render(sushi::ProxyRenderer renderer) override;
    void on_stop() override;
};


#endif //SPOOKYSUSHI_GAME_HPP
