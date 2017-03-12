#version 430 core

out vec4 color;
in vec3 pos;

void main() {
    color = vec4(vec3(length(pos)), 1.0f);
}
