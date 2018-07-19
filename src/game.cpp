#include "game.hpp"
#include <rendering/static_mesh_builder.hpp>
#include <service/static_mesh_builder_service.hpp>
#include <service/service_locator.hpp>
#include <service/input_bus_service.hpp>
#include <input/input_event.hpp>
#include <opengl/opengl.hpp>
#include <input/input_state.hpp>
#include <input/input_processor.hpp>

#include <fstream>
#include <input/axis_input_processor.hpp>

sushi::StaticMeshPtr g_cube_mesh;
sushi::gl::program g_program;

sushi::MouseStateInputProcessor shoot_processor(sushi::MouseButton::Left);
sushi::KeyAxisInputProcessor move_forward_processor(sushi::Key::W, sushi::Key::S);
sushi::KeyAxisInputProcessor move_side_processor(sushi::Key::A, sushi::Key::D);
sushi::MouseAxisInputProcessor yaw_camera_processor(sushi::MouseAxis::Horizontal);
sushi::MouseAxisInputProcessor pitch_camera_processor(sushi::MouseAxis::Vertical);

void Game::on_start() {
    sushi::StaticMeshDefinition static_mesh;

    static_mesh.add({-1.0f,-1.0f,-1.0f});
    static_mesh.add({-1.0f,-1.0f, 1.0f});
    static_mesh.add({-1.0f, 1.0f, 1.0f});
    static_mesh.add({ 1.0f, 1.0f,-1.0f});
    static_mesh.add({-1.0f,-1.0f,-1.0f});
    static_mesh.add({-1.0f, 1.0f,-1.0f});
    static_mesh.add({ 1.0f,-1.0f, 1.0f});
    static_mesh.add({-1.0f,-1.0f,-1.0f});
    static_mesh.add({ 1.0f,-1.0f,-1.0f});
    static_mesh.add({ 1.0f, 1.0f,-1.0f});
    static_mesh.add({ 1.0f,-1.0f,-1.0f});
    static_mesh.add({-1.0f,-1.0f,-1.0f});
    static_mesh.add({-1.0f,-1.0f,-1.0f});
    static_mesh.add({-1.0f, 1.0f, 1.0f});
    static_mesh.add({-1.0f, 1.0f,-1.0f});
    static_mesh.add({ 1.0f,-1.0f, 1.0f});
    static_mesh.add({-1.0f,-1.0f, 1.0f});
    static_mesh.add({-1.0f,-1.0f,-1.0f});
    static_mesh.add({-1.0f, 1.0f, 1.0f});
    static_mesh.add({-1.0f,-1.0f, 1.0f});
    static_mesh.add({ 1.0f,-1.0f, 1.0f});
    static_mesh.add({ 1.0f, 1.0f, 1.0f});
    static_mesh.add({ 1.0f,-1.0f,-1.0f});
    static_mesh.add({ 1.0f, 1.0f,-1.0f});
    static_mesh.add({ 1.0f,-1.0f,-1.0f});
    static_mesh.add({ 1.0f, 1.0f, 1.0f});
    static_mesh.add({ 1.0f,-1.0f, 1.0f});
    static_mesh.add({ 1.0f, 1.0f, 1.0f});
    static_mesh.add({ 1.0f, 1.0f,-1.0f});
    static_mesh.add({-1.0f, 1.0f,-1.0f});
    static_mesh.add({ 1.0f, 1.0f, 1.0f});
    static_mesh.add({-1.0f, 1.0f,-1.0f});
    static_mesh.add({-1.0f, 1.0f, 1.0f});
    static_mesh.add({ 1.0f, 1.0f, 1.0f});
    static_mesh.add({-1.0f, 1.0f, 1.0f});
    static_mesh.add({ 1.0f,-1.0f, 1.0f});

    g_cube_mesh = sushi::StaticMeshBuilderService::get().build(static_mesh);

    std::ifstream vertex_shader_source("asset/shader/default_vertex.glsl");
    std::ifstream fragment_shader_source("asset/shader/default_fragment.glsl");

    sushi::gl::vertex_shader vertex_shader;
    vertex_shader.compile(vertex_shader_source);

    sushi::gl::fragment_shader fragment_shader;
    fragment_shader.compile(fragment_shader_source);

    g_program = sushi::gl::program::make();
    g_program.attach(vertex_shader);
    g_program.attach(fragment_shader);
    g_program.link();

    sushi::InputProcessorService::get().register_processor(&shoot_processor);
    sushi::InputProcessorService::get().register_processor(&move_forward_processor);
    sushi::InputProcessorService::get().register_processor(&move_side_processor);
    sushi::InputProcessorService::get().register_processor(&yaw_camera_processor);
    sushi::InputProcessorService::get().register_processor(&pitch_camera_processor);

    //main_camera.eye_position.z = 10.f;
}

void Game::on_frame(sushi::frame_duration last_frame) {
    const sushi::Vec3 forward_movement = main_camera.forward() * move_forward_processor.value() * 0.1f;
    const sushi::Vec3 right_movement = main_camera.right() * move_side_processor.value() * 0.1f;

    const float yaw_value = yaw_camera_processor.value();
    const float YAW_RATIO = (glm::pi<float>() / 8.0f) / 100.0f; // pi/2 per 100 horizontal pixel
    const float yaw_rad_value = yaw_value * -YAW_RATIO;

    const float pitch_value = pitch_camera_processor.value();
    const float PITCH_RATIO = (glm::pi<float>() / 8.0f) / 100.0f; // pi/2 per 100 horizontal pixel
    const float pitch_rad_value = pitch_value * PITCH_RATIO;

    main_camera
            .translate(forward_movement + right_movement)
            .rotate(-pitch_rad_value, -yaw_rad_value);
}

void Game::on_late_frame(sushi::frame_duration last_frame) {

}

void Game::on_render(sushi::ProxyRenderer renderer) {
    sushi::gl::bind(g_program);

    auto projection_view_model_uniform = g_program.find_uniform<sushi::Mat4x4>("ProjectionViewModel");
    projection_view_model_uniform.set(main_camera.projection() * main_camera.view());
    g_cube_mesh->render();
}

void Game::on_stop() {

}