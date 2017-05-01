#version 430 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 normalVec;

uniform vec3 camPos;
uniform vec3 albedo;

uniform float metallic;
uniform float roughness;

uniform samplerCube irradianceMap;
uniform sampler2D lut;

const float PI = 3.14159265359;

vec3 F_Shlick(vec3 F0, float NoV) {
    return F0 + (1 - F0) * pow((1 - NoV), 5);
}

void main() {
    vec3 N = normalize(normalVec);
    vec3 V = normalize(camPos - fragPos);

    float NoV = max(dot(N, V), 0.0f);
    vec3 R = 2 * dot(N, V) * N - V; // just use reflect()?

    vec3 PrefilteredColor = textureLod(irradianceMap, R, (roughness * 3.0f)).rgb;
    vec2 EnvBRDF = texture(lut, vec2(roughness, NoV)).rg;

    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);

    vec3 k_s = F_Shlick(F0, NoV);
    vec3 k_d = (1 - metallic) * (vec3(1.0f) - k_s);

    vec3 Lambert = albedo / PI;
    vec3 BRDF = PrefilteredColor * (albedo * EnvBRDF.x + EnvBRDF.y);

    vec3 color = k_d * Lambert + k_s * BRDF;

    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f));

    FragColor = vec4(color, 1.0f);
}
