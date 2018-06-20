#version 330 core

layout(location = 0) in vec3 local_vert_position;

void main() {
    gl_Position.xyz = local_vert_position;
    gl_Position.w = 1.0;
}
