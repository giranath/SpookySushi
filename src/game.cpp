#include <fstream>
#include "game.hpp"
#include "../engine/rendering/static_mesh_builder.hpp"
#include "../engine/service/static_mesh_builder_service.hpp"
#include "../engine/opengl/opengl.hpp"

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

}

void game::on_late_frame(sushi::frame_duration last_frame) {

}

void game::on_render(sushi::ProxyRenderer renderer) {
    sushi::gl::bind(g_program);
    g_cube_mesh->render();
}

void game::on_stop() {

}