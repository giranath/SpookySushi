#version 330 core

layout(location = 0) in vec3 local_vert_position;

uniform mat4 ProjectionViewModel;

void main() {
    gl_Position = ProjectionViewModel * vec4(local_vert_position, 1.0);
}
