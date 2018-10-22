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
    physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3(0.f, 0.f, 0.f), glm::angleAxis(0.f, sushi::Vec3(0.f, 1.f, 0.f))),
                           sushi::PhysicPlaneShape(sushi::Vec3(0.f, 1.f, 0.f)));

    wrecking_ball_body = physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3{20.f, 15.f, 10.f}, glm::angleAxis(0.79f, sushi::Vec3{0.f, 0.f, 1.f})),
                                                sushi::PhysicSphereShape(1.f), 10.f);
    wrecking_ball_body.set_linear_damping(0.1f);

    auto support = physic.make_rigid_body(sushi::PhysicTransform(sushi::Vec3{10.f, 15.f, 10.f}, glm::angleAxis(0.f, sushi::Vec3(0.f, 1.f, 0.f))),
                                          sushi::PhysicBoxShape(1.f, 1.f, 5.f), 0.f);

    auto j = physic.join(wrecking_ball_body, support, sushi::PhysicRopeJoint(10.f));

    physic.destroy(j);
    physic.destroy(wrecking_ball_body);
}

void Game::prepare_scene() {
    main_camera.local_transform.translate(sushi::Vec3{0.f, 0.f, 10.f});
    main_camera.local_transform.look_at(sushi::Vec3{0.f, 0.f, 0.f});

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
}

void Game::on_frame(sushi::frame_duration last_frame) {
    update_physic(last_frame);

    const sushi::Vec3 forward_movement = main_camera.local_transform.forward() * controller.get_move_forward_value() * 0.1f;
    const sushi::Vec3 right_movement = main_camera.local_transform.right() * controller.get_move_strate_value() * 0.1f;

    const float yaw_value = -controller.get_camera_yaw_value();
    const float YAW_RATIO = (glm::pi<float>() / 8.0f) / 100.0f; // pi/2 per 100 horizontal pixel
    const float yaw_rad_value = yaw_value * YAW_RATIO;
    const sushi::Quaternion yaw_quat = glm::angleAxis(yaw_rad_value, sushi::Vec3{0.f, 1.f, 0.f});

    const float pitch_value = controller.get_camera_pitch_value();
    const float PITCH_RATIO = (glm::pi<float>() / 8.0f) / 100.0f; // pi/2 per 100 horizontal pixel
    const float pitch_rad_value = pitch_value * PITCH_RATIO;
    const sushi::Quaternion pitch_quat = glm::angleAxis(pitch_rad_value, sushi::Vec3{1.f, 0.f, 0.f});

    if(controller.should_apply_boost() && wrecking_ball_body) {
        auto wrecking_ball_transform = wrecking_ball_body.transform();

        const sushi::Vec3 forward(1.f, 0.f, 0.f);
        const sushi::Vec3 rotated_forward = glm::normalize(wrecking_ball_transform.rotation * forward);

        wrecking_ball_body.apply_force_at(sushi::Vec3{1.f, 0.f, 0.f}, rotated_forward * 10.f);
    }

    main_camera.local_transform
            .translate(forward_movement + right_movement)
            .set_rotation(yaw_quat * main_camera.local_transform.rotation() * pitch_quat);
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