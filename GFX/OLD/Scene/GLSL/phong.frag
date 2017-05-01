#version 430 core

out vec4 color;

in vec3 fragPos;
in vec2 texCoord;
in vec3 normalVec; // positive Z in plain-space

uniform vec3 camPos;
uniform vec3 lightPos;

uniform sampler2D diffuseMap;
uniform sampler2D specularMap;

mat3 rotationMatrix(vec3 axis, float angle) {
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat3(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c);
}

vec3 CalcPointLight(vec3 lightPos, vec3 viewDir) {
    vec3 lightColor = vec3(1.0f);

    vec3 normal = normalize(normalVec);

    float lightAmbient = 0.1f;
    float lightDiffuse = 0.9f;
    float lightSpecular = 0.8f;

    vec3 lightDir = normalize(lightPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);

    float diff = max(dot(normal, lightDir), 0.0f);
    float spec = pow(max(dot(normal, halfwayDir), 0.0f), 16);

    spec *= diff;

    vec3 ambient = vec3(lightAmbient);
    vec3 diffuse = lightDiffuse * (lightAmbient + diff) * vec3(texture(diffuseMap, texCoord));
    vec3 specular = lightSpecular * spec * vec3(texture(specularMap, texCoord));

    float k0 = 1.0f, k1 = 0.01f, k2 = 0.075f;
    float d = length(lightPos - fragPos);
    float distscale = 1.0f / (k0 + k1 * d + k2 * d * d);

    return distscale * (diffuse + specular);
}


void main() {
    vec3 objectColor = vec3(1.0f, 0.5f, 0.4f);
    vec3 viewDir = normalize(camPos - fragPos);

    vec3 lighting = CalcPointLight(lightPos, viewDir);

    color = vec4(lighting, 1.0f);
}
