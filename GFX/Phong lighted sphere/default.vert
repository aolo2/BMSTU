#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 normalVec;
out vec3 fragPos;

void main() {
    gl_Position = proj * view * model * vec4(position, 1.0);
    fragPos = vec3(model * vec4(position, 1.0f));
    vec3 normal = normalize(vec3(position));
    normalVec = mat3(transpose(inverse(model))) * normal;
}