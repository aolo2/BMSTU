#version 430 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D canvas;

void main() {
    color = texture(canvas, texCoords);
}