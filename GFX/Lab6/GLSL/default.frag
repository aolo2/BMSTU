#version 430 core

layout (location = 0) out vec4 color;

in vec3 fragPos;
in vec3 normalVec;

uniform vec3 lightPos;

const float diffuse = 0.8f;
const float specular = 0.3f;

vec3 Phong_d(vec3 L, vec3 N) {
    return diffuse * max(dot(L, N), 0.0f);
}

float attenuation(float dist, float k0, float k1, float k2) {
    return (1.0f) / (k0 + k1 * dist + k2 * dist * dist);
}

void main() {
    vec3 baseColor = vec3(1.0f, 0.0f, 0.0f);
    vec3 ambient = vec3(0.1f);

    vec3 L = normalize(lightPos - fragPos);    // light direction vector
    vec3 N = normalize(normalVec);             // normal vector

    vec3 lighting = Phong_d(L, N);
    float dist = length(lightPos - fragPos);
    lighting *= attenuation(1.0f, 0.2f, 0.3f, dist);

    color = vec4(baseColor * (ambient + lighting), 1.0f);
}
