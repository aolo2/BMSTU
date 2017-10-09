#version 450 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec3 color;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out VS_OUT {
    vec3 color;
    vec3 fragPos;
    vec3 normal;
} vs_out;

void main() {
    vs_out.color = color;
    vs_out.normal = transpose(inverse(mat3(model))) * normal;
    vs_out.fragPos = vec3(model * vec4(position, 1.0f));

    gl_Position = proj * view * model * vec4(position.x, position.y, position.z, 1.0f);
}