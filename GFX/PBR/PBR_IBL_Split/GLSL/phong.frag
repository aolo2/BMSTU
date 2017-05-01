#version 430 core

out vec4 color;

in vec3 fragPos;
in vec3 normalVec;

uniform vec3 camPos;
uniform vec3 lightPos;
uniform float diffuse;
uniform float specular;

vec3 Phong_d(vec3 L, vec3 N) {
    return diffuse * max(dot(L, N), 0.0f);
}

vec3 Phong_s(vec3 N, vec3 H, vec3 L) {
    if (dot(N, L) <= 0) { return vec3(0.0f); }
    return specular * pow(max(dot(H, N), 0.0f), 64);
}

void main() {
    vec3 baseColor = vec3(1.0f, 0.0f, 0.0f);
    vec3 ambient = vec3(0.1f);

    vec3 L = normalize(lightPos - fragPos);    // light direction vector
    vec3 V = normalize(camPos - fragPos);      // view direction vector
    vec3 N = normalize(normalVec);             // normal vector
    vec3 H = normalize(L + V);                 // halfway vector

    vec3 lighting = Phong_d(L, N);
    lighting += Phong_s(N, H, L);

    color = vec4(baseColor * (ambient + lighting), 1.0f);
}
