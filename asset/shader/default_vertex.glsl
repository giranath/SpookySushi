#version 330 core

layout(location = 0) in vec3 local_vert_position;
layout(location = 1) in vec3 local_normal;

uniform mat4 ProjectionViewModel;
uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

out vec3 normal;
out vec3 frag_position;

void main() {
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(local_vert_position, 1.0);
    frag_position = vec3(model_matrix * vec4(local_vert_position, 1.0));
    normal = local_normal;
}
