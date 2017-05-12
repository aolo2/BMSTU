#version 430 core

layout (location = 0) out vec4 color;

in vec3 fragPos;
in vec3 normalVec;
in vec2 texCoord;

uniform int textured;
uniform vec3 lightPos;
uniform sampler2D albedo;

uniform float diffuse;

float Phong_d(vec3 L, vec3 N) {
    return diffuse * max(dot(L, N), 0.0f);
}

float attenuation(float dist, float k0, float k1, float k2) {
    return (1.0f) / (k0 + k1 * dist + k2 * dist * dist);
}

void main() {
    vec3 baseColor;
    
    baseColor = ((textured == 1) ? texture(albedo, texCoord).rgb : vec3(0.7f));
    vec3 ambient = vec3(0.1f);

    vec3 L = normalize(lightPos - fragPos);    // light direction vector
    vec3 N = normalize(normalVec);             // normal vector

    float lighting = Phong_d(L, N);
    float dist = length(lightPos - fragPos);
    lighting *= attenuation(1.0f, 0.1f, 0.15f, dist);

    color = vec4(baseColor * (ambient + lighting), 1.0f);
}
