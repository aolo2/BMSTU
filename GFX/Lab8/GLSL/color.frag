#version 430 core

layout (location = 0) out vec4 FragColor;

in vec3 Color;
in vec3 Normal;
in vec3 Fragpos;

void main() {
    FragColor = vec4(Color + (Normal * 0.2f), 1.0f);
}