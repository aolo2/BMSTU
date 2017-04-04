#version 430 core

out vec4 color;
in vec3 pos;

void main() {
    color = vec4(abs(normalize(pos)), 1.0f);
    // color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
