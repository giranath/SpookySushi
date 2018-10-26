#include "game.hpp"

#include <static_mesh_builder.hpp>
#include <static_mesh_builder_service.hpp>
#include <service_locator.hpp>
#include <input_bus_service.hpp>
#include <debug_draw_service.hpp>
#include <input_event.hpp>
#include <opengl.hpp>
#include <input_state.hpp>
#include <input_processor.hpp>
#include <axis_input_processor.hpp>
#include <sdl_event.hpp>
#include <package.hpp>
#include <log_service.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <fstream>
#include <string>
#include <sstream>

sushi::StaticMeshDefinition make_cube(const float size) {
    sushi::StaticMeshDefinition static_mesh;

    static_mesh.add({-size, -size, -size});
    static_mesh.add({ size, -size, -size});
    static_mesh.add({ size,  size, -size});
    static_mesh.add({-size,  size, -size});
    static_mesh.add({-size, -size,  size});
    static_mesh.add({ size, -size,  size});
    static_mesh.add({ size,  size,  size});
    static_mesh.add({-size,  size,  size});

    // Add all the indices
    static_mesh.add_triangle_indices(0, 2, 1);
    static_mesh.add_triangle_indices(0, 3, 2);
    static_mesh.add_triangle_indices(4, 6, 5);
    static_mesh.add_triangle_indices(4, 7, 6);
    static_mesh.add_triangle_indices(0, 7, 4);
    static_mesh.add_triangle_indices(0, 3, 7);
    static_mesh.add_triangle_indices(1, 6, 5);
    static_mesh.add_triangle_indices(1, 2, 6);
    static_mesh.add_triangle_indices(3, 7, 6);
    static_mesh.add_triangle_indices(3, 6, 2);
    static_mesh.add_triangle_indices(0, 4, 5);
    static_mesh.add_triangle_indices(0, 5, 1);

    return static_mesh;
}

void Game::prepare_shader() {
    std::ifstream vertex_shader_source("asset/shader/default_vertex.glsl");
    std::ifstream fragment_shader_source("asset/shader/default_fragment.glsl");

    sushi::gl::vertex_shader vertex_shader;
    auto res = vertex_shader.compile(vertex_shader_source);
    if(!res) {
        sushi::log_warning("sushi.game.start", "failed to compile vertex shader: %s", res.message().c_str());
    }

    sushi::gl::fragment_shader fragment_shader;
    res = fragment_shader.compile(fragment_shader_source);
    if(!res) {
        sushi::log_warning("sushi.game.start", "failed to compile fragment shader: %s", res.message().c_str());
    }

    default_shader = sushi::gl::program::make();
    default_shader.attach(vertex_shader);
    default_shader.attach(fragment_shader);
    auto link_res = default_shader.link();
    if(!link_res) {
        sushi::log_warning("sushi.game.start", "failed to link program: %s", link_res.message().c_str());
    }
}

void Game::prepare_physic_scene() {
    // Sol
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(0.f, 0.f, 0.f), glm::angleAxis(0.f, sushi::Vec3(0.f, 1.f, 0.f))),
                           sushi::PhysicPlaneShape(sushi::Vec3(0.f, 1.f, 0.f)));

    wrecking_ball_body = physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3{0.f, 10.f, 0.f}),
                                                sushi::PhysicSphereShape(1.f), 10.f);
    wrecking_ball_body.set_linear_damping(0.1f);
    wrecking_ball_body.set_angular_damping(0.6);
    sushi::MovementLock wrecking_lock;
    wrecking_lock.lock(sushi::RotationLock::Pitch).lock(sushi::RotationLock::Yaw).lock(sushi::RotationLock::Roll);
    wrecking_ball_body.set_movement_lock(wrecking_lock);

    // Add 5 towers
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(0.f, 10.f, 0.f)), sushi::PhysicBoxShape(5.f, 20.f, 5.f)).set_query_filter_mask(1);
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(-50.f, 10.f, 30.f)), sushi::PhysicBoxShape(5.f, 20.f, 5.f)).set_query_filter_mask(1);
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(-30.f, 10.f, 10.f)), sushi::PhysicBoxShape(5.f, 20.f, 5.f)).set_query_filter_mask(1);
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(-70.f, 10.f, -30.f)), sushi::PhysicBoxShape(5.f, 20.f, 5.f)).set_query_filter_mask(1);
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(-110.f, 10.f, 0.f)), sushi::PhysicBoxShape(5.f, 20.f, 5.f)).set_query_filter_mask(1);
}

void Game::prepare_scene() {
    main_camera.local_transform.reset();
    main_camera.local_transform.set_translation(sushi::Vec3{40.f, 16.f, 10.f});

    // Extract monkey mesh from package
    sushi::Package package;
    std::ifstream package_stream{"asset/generated/default.spkg", std::ios::binary};
    package_stream >> package;

    auto monkey_model_view = package.entry_data(0, sushi::ValidateEntry{});
    if(monkey_model_view) {
        std::istringstream monkey_model_stream(std::string{*monkey_model_view});
        sushi::MeshAsset asset;
        monkey_model_stream >> asset;

        mesh = sushi::StaticMeshBuilderService::get().build(sushi::StaticMeshDefinition{asset});
    }
    else {
        sushi::log_critical("sushi.game", "cannot get model from package");
    }

    prepare_physic_scene();
}

void Game::on_start() {
    controller.register_inputs();
    camera_controller.set_camera(&main_camera);
    camera_controller.set_target(&player_transform);
    physic_update_elapsed_time = 0;

    prepare_shader();
    prepare_scene();

    //sushi::set_relative_mouse_mode(true);
}

void Game::update_physic(sushi::frame_duration last_frame) {
    physic_update_elapsed_time += std::chrono::duration_cast<std::chrono::milliseconds>(last_frame).count();
    while(physic_update_elapsed_time > 16) {
        physic.step_simulation(1.f / 60.f);
        physic_update_elapsed_time -= 16;
    }

    // Update player's transform
    const sushi::PhysicTransform new_player_transform = wrecking_ball_body.transform();
    player_transform.set_translation(new_player_transform.translation);
    player_transform.set_rotation(new_player_transform.rotation);
}

void Game::on_frame(sushi::frame_duration last_frame) {
    update_physic(last_frame);

    // Update camera
    camera_controller.update(last_frame);

    // Player controls
    if(controller.should_apply_boost() && wrecking_ball_body) {
        auto wrecking_ball_transform = wrecking_ball_body.transform();

        wrecking_ball_body.apply_force_at(sushi::Vec3{0.f, 0.f, 0.f}, 
                                          wrecking_ball_transform.to_transform().forward() * 50.f);
    }

    const float move_forward = controller.get_move_forward_value();
    const float move_right = controller.get_move_strate_value();

    const bool have_forward_movement = move_forward > 0.f;
    const bool have_right_movement = move_right > 0.f;

    const bool have_movement = have_forward_movement || have_right_movement;

    const sushi::Vec3 forward_movement = wrecking_ball_body.transform().to_transform().forward() * move_forward;
    const sushi::Vec3 right_movement = wrecking_ball_body.transform().to_transform().right() * move_right;

    sushi::Vec3 normalized_direction = forward_movement + right_movement;
    if(have_movement) {
        normalized_direction = glm::normalize(normalized_direction);
    }

    wrecking_ball_body.apply_force_at(sushi::Vec3{}, normalized_direction * 10.f);

    if(controller.should_shoot_left_grappling()) {
        const sushi::MouseCoords coords = sushi::current_mouse_coords();
        const float viewport_x = ((2.f * coords.x) / 1024.f) - 1.f;
        const float viewport_y = 1.f - (2.f * coords.y) / 768.f;

        const auto world_ray = main_camera.ray_from_viewport(sushi::Vec2{viewport_x, viewport_y});
        const auto temp = main_camera.local_transform.forward();

        auto raycast = physic.raycast(world_ray.origin, world_ray.direction, 1000.f, 1);

        if(raycast) {
            if (left_grappling_joint) {
                physic.destroy(left_grappling_joint);
            }

            left_grappling_joint = physic.join(wrecking_ball_body, raycast.rigidbody(), sushi::PhysicRopeJoint(raycast.distance()));
            left_grappling_joint.enable_collision();
            sushi::DebugRendererService::get().add_sphere(raycast.rigidbody().transform().translation, 0.25f, sushi::Colors::Orange, 1000);
        }
    }

    if (left_grappling_joint) {
        sushi::DebugRendererService::get().add_line(wrecking_ball_body.transform().translation, left_grappling_joint.rigid_bodies().second.transform().translation, sushi::Colors::Yellow);
    }

    if(controller.should_shoot_right_grappling()) {
        auto raycast = physic.raycast(main_camera.local_transform.translation(), main_camera.local_transform.forward(), 1000.f, 1);

        if(raycast) {
            if (right_grappling_joint) {
                physic.destroy(right_grappling_joint);
            }

            right_grappling_joint = physic.join(wrecking_ball_body, raycast.rigidbody(), sushi::PhysicRopeJoint(raycast.distance()));
            right_grappling_joint.enable_collision();
            sushi::DebugRendererService::get().add_sphere(raycast.rigidbody().transform().translation, 0.25f, sushi::Colors::Red, 1000);
        }
    }
}

void Game::on_late_frame(sushi::frame_duration last_frame) {

}

void Game::on_render(sushi::ProxyRenderer renderer) {
    renderer.set_active_camera(&main_camera);

    sushi::gl::bind(default_shader);

    auto projection_matrix_uniform = default_shader.find_uniform<sushi::Mat4x4>("projection_matrix");
    auto view_matrix_uniform = default_shader.find_uniform<sushi::Mat4x4>("view_matrix");
    auto model_matrix_uniform = default_shader.find_uniform<sushi::Mat4x4>("model_matrix");
    auto model_color = default_shader.find_uniform<sushi::RGBColor>("model_color");

    projection_matrix_uniform.set(main_camera.projection());
    view_matrix_uniform.set(main_camera.view());
    model_matrix_uniform.set(glm::scale(sushi::Mat4x4{1.f}, glm::vec3{0.001f, 0.001f, 0.001f}));
    model_color.set(sushi::Colors::Yellow);

    if(mesh) {
        mesh->render();
    }

    sushi::draw_physic_debug(physic);
}

void Game::on_stop() {
    sushi::set_relative_mouse_mode(false);
}