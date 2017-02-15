#version 430 core

out vec4 outColor;
uniform vec4 color;

void main() {
    outColor = vec4(color.x / 255, color.y / 255, color.z / 255, 1.0f);
}