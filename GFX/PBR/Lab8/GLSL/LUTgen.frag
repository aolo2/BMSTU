#version 430 core

out vec4 FragColor;

in vec2 coords;

const float PI = 3.14159265359f;

float G_ShlickGGX(float NoV, float K) {
    float nom = NoV;
    float denom = NoV * (1 - K) + K;

    return nom / denom;
}

float G_Smith(float NoV, float NoL, float roughness) {
    float K = roughness * roughness / 2;
    return G_ShlickGGX(NoV, K) * G_ShlickGGX(NoL, K);
}

vec2 Hammersley(int i, int N) {
    return vec2(float(i) / float(N), float(bitfieldReverse(i)) * 2.3283064365386963e-10);
}

vec3 ImportanceSampleGGX(vec2 Xi, float roughness, vec3 N) {
    float a = roughness * roughness; // IBL 'a' term
    float phi = 2 * PI * Xi.x;

    float cosT = sqrt((1 - Xi.y) / (1 + (a * a - 1) * Xi.y));
    float sinT = sqrt(1 - cosT * cosT);

    vec3 H;
    H.x = sinT * cos(phi);
    H.y = sinT * sin(phi);
    H.z = cosT;

    vec3 up = abs(N.z) < 0.999f ? vec3(0.0f, 0.0f, 1.0f) : vec3(1.0f, 0.0f, 0.0f);
    vec3 tanX = normalize(cross(up, N));
    vec3 tanY = cross(N, tanX);

    return normalize(tanX * H.x + tanY * H.y + N * H.z);
}

vec2 IntegrateBRDF(float roughness, float NoV, vec3 N) {
    vec3 V;
    V.x = sqrt(1.0f - NoV * NoV);
    V.y = 0;
    V.z = NoV;

    float A = 0.0f;
    float B = 0.0f;

    const int samples = 1024;
    for (int i = 0; i < samples; i++) {

        vec2 Xi = Hammersley(i, samples);
        vec3 H = ImportanceSampleGGX(Xi, roughness, N);
        vec3 L = normalize(2.0f * dot(V, H) * H - V);

        float NoL = max(L.z, 0.0f);
        float NoH = max(H.z, 0.0f);
        float VoH = max(dot(V, H), 0.0f);

        if (NoL > 0) {
            float G = G_Smith(NoV, NoL, roughness); // V, L? L, V?
            float G_vis = G * VoH / (NoH * NoV);
            float Fc = pow(1 - VoH, 5);

            A += (1.0f - Fc) * G_vis;
            B += Fc * G_vis;
        }
    }

    return vec2(A, B) / float(samples);
}

void main() {
    vec3 N = vec3(0.0f, 0.0f, 1.0f);
    vec2 color = IntegrateBRDF(coords.x, coords.y, N);
    
    // for integrity
    FragColor = vec4(color, 0.0f, 1.0f);
}
