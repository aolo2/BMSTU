#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 fragPos;
out vec2 texCoord;
out vec3 normalVec;

void main() {
    gl_Position = proj * view * model * vec4(position, 1.0);

    normalVec = mat3(transpose(inverse(model))) * normal;
    fragPos = vec3(model * vec4(position, 1.0f));
    texCoord = texcoord;
}
