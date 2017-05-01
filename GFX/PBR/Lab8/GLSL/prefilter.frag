#version 430 core

out vec4 fragColor;
in vec3 localPos;

uniform samplerCube environmentMap;
uniform float roughness;

const float PI = 3.14159265359f;

vec3 ImportanceSample(vec2 Xi, float roughness, vec3 N) {
    float a = roughness * roughness;
    float phi = 2 * PI * Xi.x;

    float cosT = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
    float sinT = sqrt(1 - cosT * cosT);

    vec3 H;
    H.x = sinT * cos(phi);
    H.y = sinT * sin(phi);
    H.z = cosT;

    vec3 up = abs(N.z) > 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
    vec3 tanX = normalize(cross(up, N));
    vec3 tanY = cross(N, tanX);

    return tanX * H.x + tanY * H.y + N * H.z;
}

vec2 Hammersley(int i, int N) {
    return vec2(float(i) / float(N), float(bitfieldReverse(i)) * 2.3283064365386963e-10);
}

void main() {
    vec3 N = normalize(localPos);
    vec3 V = N;

    vec3 color = vec3(0.0f);
    float weight = 0.0f;

    const int samples = 1024;

    for (int i = 0; i < samples; i++) {
        vec2 Xi = Hammersley(i, samples);
        vec3 H = ImportanceSample(Xi, roughness, N);
        vec3 L = 2 * dot(V, H) * H - V;

        float NoL = max(dot(N, L), 0.0f);

        if (NoL > 0) {
            color += texture(environmentMap, L).rgb * NoL;
            weight += NoL;
        }
    }

    fragColor = vec4(color / weight, 1.0f);
    // fragColor = vec4(1.0f);
}
