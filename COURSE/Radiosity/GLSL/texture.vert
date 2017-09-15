#version 450 core

layout (location = 0) in vec2 position;

out vec2 texCoords;

void main() {
    gl_Position = vec4(position, 1.0f, 1.0f);
    texCoords = (position + 1) / 2.0f;
}