#version 430 core

in vec2 coords;
out vec4 color;

uniform sampler2D lut;

void main() {
    color = vec4(texture(lut, coords).rgb, 1.0f);
}
