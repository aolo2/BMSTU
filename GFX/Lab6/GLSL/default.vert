#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 proj;

out vec3 fragPos;
out vec3 normalVec;

void main() {
    gl_Position = proj * model * vec4(position, 1.0f);
    fragPos = vec3(model * vec4(position, 1.0f));
    normalVec = transpose(inverse(mat3(model))) * normal;
}