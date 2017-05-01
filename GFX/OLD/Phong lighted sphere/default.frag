#version 430 core

out vec4 color;

in vec3 normalVec;
in vec3 fragPos;

struct DirLight {
    vec3 direction;
    vec3 color;

    float ambient, diffuse, specular;
};  

struct PointLight {
    vec3 position;
    vec3 color;
    
    float ambient, diffuse, specular;
    float k0, k1, k2;
};

struct ConeLight {
    vec3 position, direction;
    vec3 color;
    
    float k0, k1, k2;
    float ambient, diffuse, specular;
    float hardCutOff, softCutOff;
};

uniform DirLight dirLight;
uniform PointLight pointLight;
uniform ConeLight coneLight;
uniform vec3 camPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float diff = max(dot(normal, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    
    return light.color * vec3(light.ambient + (light.diffuse * diff));// + (light.specular * spec));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float diff = max(dot(normal, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    
    float d = length(light.position - fragPos);
    float distScale = 1.0f / (light.k0 + light.k1 * d + light.k2 * d * d);
    
    return light.color * distScale * vec3(light.ambient + (light.diffuse * diff) + (light.specular * spec));
}

vec3 CalcConeLight(ConeLight light, vec3 viewDir) {
    float theta = dot(viewDir, normalize(-coneLight.direction));
    float diff = max(dot(normalVec, viewDir), 0.0);
    
    vec3 lightDir = normalize(camPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normalVec);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 16);
    
    float d = length(camPos - fragPos);
    float distScale = 1.0f / (light.k0 + light.k1 * d + light.k2 * d * d);
    
    float epsilon = light.hardCutOff - light.softCutOff;
    float intensity = clamp((theta - light.softCutOff) / epsilon, 0.0f, 1.0f);
    
        
    return light.color * intensity * distScale * vec3((light.diffuse * diff) + (light.specular * spec));
}

void main() {
    vec3 resColor, lighting;
    vec3 objectColor = vec3(1.0f, 0.5f, 0.0f);
    vec3 viewDir = normalize(camPos - fragPos);

    lighting = CalcDirLight(dirLight, normalVec, viewDir);
    lighting += CalcPointLight(pointLight, normalVec, viewDir);
    lighting += CalcConeLight(coneLight, viewDir);
        
    color = vec4(lighting * objectColor, 1.0f);
}

