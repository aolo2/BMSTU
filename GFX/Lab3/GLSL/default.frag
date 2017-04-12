#version 330 core

out vec4 color;
in vec3 pos;

void main() {
    color = vec4(abs(normalize(pos)), 1.0f);
}
