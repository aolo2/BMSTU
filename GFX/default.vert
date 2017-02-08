#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 transform;
uniform mat4 revert;

void main() {
    gl_Position = transform * revert * vec4(position, 1.0);
}