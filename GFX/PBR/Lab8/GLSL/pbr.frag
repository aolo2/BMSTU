#version 430 core

out vec4 FragColor;

in vec3 fragPos;
in vec3 normalVec;

uniform vec3 camPos;

uniform float roughness;
uniform float metallic;

uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D BRDFlut;

const float PI = 3.14159265359;

vec3 F_Shlick(float NoV, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0f - roughness), F0) - F0) * pow(1.0 - NoV, 5.0f);
}

void main() {
    vec3 albedo = vec3(1.0f);

    vec3 N = normalize(normalVec);
    vec3 V = normalize(camPos - fragPos);
    vec3 R = reflect(-V, N);

    float NoV = max(dot(N, V), 0.0f);

    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);

    vec3 F = F_Shlick(NoV, F0, roughness);
    vec3 k_s = F;
    vec3 k_d = vec3(1.0f) - k_s;
    k_d *= (1.0f - metallic);

    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;

    const float MAX_LOD = 3.0f;
    vec3 prefilteredColor = textureLod(prefilterMap, R, roughness * MAX_LOD).rgb;
    vec2 BRDF = texture(BRDFlut, vec2(roughness, NoV)).rg;
    vec3 specular = prefilteredColor * (F * BRDF.x + BRDF.y);

    vec3 ambient = k_d * diffuse + specular; // * ao
    vec3 color = ambient;

    // "HDR"
    color = color / (color + vec3(1.0f));
    color = pow(color, vec3(1.0f / 2.2f));

    FragColor = vec4(color, 1.0f);
}
