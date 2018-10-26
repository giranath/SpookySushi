#version 330

out vec3 color;
in vec3 normal;
in vec3 frag_position;
in vec3 frag_color;

void main() {
    vec3 ambiant_color = vec3(1, 1, 1);
    float ambiant_strength = 0.1;
    vec3 ambiant = ambiant_strength * ambiant_color;

    vec3 light_position = vec3(1000, 1000, 1000);
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - frag_position);
    vec3 light_color = vec3(1, 1, 1);

    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = diff * light_color;

    color = (ambiant + diffuse) * frag_color;
}

