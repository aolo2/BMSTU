#version 450 core

layout (location = 0) out vec3 fragColor;

in VS_OUT {
    vec3 color;
    vec3 fragPos;
    vec3 normal;
} vs_in;

// 278.0f, 0.0f, 279.5f -- light source center
const vec3 lightPos = vec3(0.0f, 0.5f, 1.0f);
// uniform vec3 lightPos;

void main() {
    vec3 lightVec = lightPos - vs_in.fragPos;
    float dist = length(lightVec);

    vec3 L = normalize(lightVec);
    vec3 N = normalize(vs_in.normal);

    fragColor = vec3(max(dot(L, N), 0.0f)) / (dist * dist + 1.0f);
}