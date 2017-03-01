#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 proj;

out vec3 texCoords;

void main() {
    vec4 pos = proj * view * vec4(position, 1.0);
    gl_Position = pos.xyww;
    texCoords = position;
}
