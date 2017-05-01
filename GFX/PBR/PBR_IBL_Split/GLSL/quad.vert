#version 430 core

layout (location = 0) in vec2 position;

out vec2 coords;

void main() {
    coords = vec2((position.x + 1.0f) * 0.5f, (position.y + 1.0f) * 0.5f);
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
}
