#include "game.hpp"
#include <rendering/static_mesh_builder.hpp>
#include <service/static_mesh_builder_service.hpp>
#include <opengl/opengl.hpp>

#include <fstream>

sushi::StaticMeshPtr g_cube_mesh;
sushi::gl::program g_program;

void game::on_start() {
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
}

void game::on_frame(sushi::frame_duration last_frame) {
    main_camera.local_position().z = -2.5f;
    main_camera.local_position().x = -2.5f;
    main_camera.local_position().y =  0.f;
}

void game::on_late_frame(sushi::frame_duration last_frame) {

}

void game::on_render(sushi::ProxyRenderer renderer) {
    sushi::gl::bind(g_program);

    auto projection_view_model_uniform = g_program.find_uniform<sushi::Mat4x4>("ProjectionViewModel");
    projection_view_model_uniform.set(main_camera.projection() * main_camera.view());
    g_cube_mesh->render();
}

void game::on_stop() {

}