#version 430 core

out vec4 color;

in vec3 fragPos;
in vec3 normalVec;

uniform vec3 lightPos;

const float diffuse = 0.8f;
const float specular = 0.3f;

vec3 Phong_d(vec3 L, vec3 N) {
    return diffuse * max(dot(L, N), 0.0f);
}

vec3 Phong_s(vec3 N, vec3 H, vec3 L) {
    if (dot(N, L) <= 0) { return vec3(0.0f); }
    return specular * pow(max(dot(H, N), 0.0f), 32);
}

float attenuation(float dist, float k0, float k1, float k2) {
    return (1.0f) / (k0 + k1 * dist + k2 * dist * dist);
}

void main() {
    vec3 baseColor = vec3(1.0f, 0.0f, 0.0f);
    vec3 ambient = vec3(0.1f);

    vec3 L = normalize(lightPos - fragPos);    // light direction vector
    // vec3 V = normalize(camPos - fragPos);      // view direction vector
    vec3 N = normalize(normalVec);             // normal vector
    // vec3 H = normalize(L + V);                 // halfway vector

    vec3 lighting = Phong_d(L, N);
    float dist = length(lightPos - fragPos);
    lighting *= attenuation(1.0f, 0.15f, 0.07f, dist);
    // lighting += Phong_s(N, H, L);

    color = vec4(baseColor * (ambient + lighting), 1.0f);
}
