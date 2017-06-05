#version 430 core

layout (location = 0) out vec4 FragColor;

in vec3 Normal;
in vec3 Fragpos;

void main() {
    FragColor = vec4(1.0f);
}