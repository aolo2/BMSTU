#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 proj;

out vec3 pos;

void main() {
    gl_Position = proj * model * vec4(position, 1.0f);
    pos = position;
}