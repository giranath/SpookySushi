#include "game.hpp"

#include <static_mesh_builder.hpp>
#include <static_mesh_builder_service.hpp>
#include <service_locator.hpp>
#include <input_bus_service.hpp>
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
        std::cerr << res.message() << std::endl;
    }

    sushi::gl::fragment_shader fragment_shader;
    res = fragment_shader.compile(fragment_shader_source);
    if(!res) {
        std::cerr << res.message() << std::endl;
    }

    default_shader = sushi::gl::program::make();
    default_shader.attach(vertex_shader);
    default_shader.attach(fragment_shader);
    default_shader.link();
}

void Game::on_start() {
    prepare_shader();

    main_camera.local_transform.translate(sushi::Vec3{0.f, 0.f, 2.f});
    main_camera.local_transform.look_at(sushi::Vec3{0.f, 0.f, 0.f});

    // Extract monkey mesh from package
    sushi::Package package;
    std::ifstream package_stream{"asset/generated/default.spkg", std::ios::binary};
    package_stream >> package;

    auto monkey_model_view = package.entry_data(0, sushi::ValidateEntry{});
    if(monkey_model_view) {
        // TODO: Automate asset extraction from package
        std::istringstream monkey_model_stream(std::string{*monkey_model_view});
        sushi::MeshAsset asset;
        monkey_model_stream >> asset;

        mesh = sushi::StaticMeshBuilderService::get().build(sushi::StaticMeshDefinition{asset});
    }
    else {
        sushi::log_critical("sushi.game", "cannot get model from package");
        throw 5;
    }

    controller.register_inputs();

    sushi::set_relative_mouse_mode(true);
}

void Game::on_frame(sushi::frame_duration last_frame) {
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

    main_camera.local_transform
            .translate(forward_movement + right_movement)
            .set_rotation(yaw_quat * main_camera.local_transform.rotation() * pitch_quat);
}

void Game::on_late_frame(sushi::frame_duration last_frame) {

}

void Game::on_render(sushi::ProxyRenderer renderer) {
    sushi::gl::bind(default_shader);

    auto projection_matrix_uniform = default_shader.find_uniform<sushi::Mat4x4>("projection_matrix");
    auto view_matrix_uniform = default_shader.find_uniform<sushi::Mat4x4>("view_matrix");
    auto model_matrix_uniform = default_shader.find_uniform<sushi::Mat4x4>("model_matrix");

    projection_matrix_uniform.set(main_camera.projection());
    view_matrix_uniform.set(main_camera.view());
    model_matrix_uniform.set(glm::scale(sushi::Mat4x4{1.f}, glm::vec3{0.01f, 0.01f, 0.01f}));

    mesh->render();
}

void Game::on_stop() {
    sushi::set_relative_mouse_mode(false);
}