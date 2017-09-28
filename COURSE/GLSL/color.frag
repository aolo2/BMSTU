#version 450 core

layout (location = 0) out vec3 fragColor;

VS_OUT {
    vec3 normal;
    vec3 fragPos;
    vec3 color;
} vs_in;

void main() {
    fragColor = vs_in.color;
}