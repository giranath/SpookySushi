#ifndef SPOOKYSUSHI_GAME_HPP
#define SPOOKYSUSHI_GAME_HPP

#include "game_controller.hpp"
#include "camera_controller.hpp"

#include <program.hpp>
#include <base_game.hpp>
#include <camera.hpp>
#include <static_mesh_builder.hpp>
#include <physic_world.hpp>

class Game : public sushi::BaseGame {
    sushi::Camera main_camera;
    GameController controller;
    CameraController camera_controller;
    sushi::gl::program default_shader;
    sushi::StaticMeshPtr mesh;
    sushi::Transform player_transform;
    sushi::PhysicWorld physic;
    uint32_t physic_update_elapsed_time;
    sushi::PhysicWorld::rigid_body_type wrecking_ball_body;

    sushi::PhysicWorld::joint_type left_grappling_joint;
    sushi::PhysicWorld::joint_type right_grappling_joint;

    void prepare_shader();

    void prepare_physic_scene();
    void prepare_scene();

    void update_physic(sushi::frame_duration last_frame);

    void handle_inputs(sushi::frame_duration last_frame);
public:
    void on_start() override;
    void on_frame(sushi::frame_duration last_frame) override;
    void on_late_frame(sushi::frame_duration last_frame) override;
    void on_render(sushi::ProxyRenderer renderer) override;
    void on_stop() override;
};


#endif //SPOOKYSUSHI_GAME_HPP
