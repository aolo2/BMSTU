#version 430 core

out vec4 color;

in vec3 normalVec;
in vec3 fragPos;

struct DirLight {
    vec3 direction, ambient, diffuse, specular, color;
};  

struct PointLight {    
    vec3 position, ambient, diffuse, specular, color;
    vec3 k0k1k2;
};  

struct ConeLight {
    vec3 color, direction, k0k1k2;
    float hardAngle, softAngle, diffuse;
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
    
    return light.ambient + (light.diffuse * diff); //(light.specular * spec);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    
    float diff = max(dot(normal, lightDir), 0.0f);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    
    return light.ambient + (light.diffuse * diff) + (light.specular * spec);
}

vec3 CalcConeLight(ConeLight light, vec3 viewDir) {
    float cosine = dot(viewDir, normalize(-coneLight.direction));
    
    if (cosine > light.softAngle) {
        float epsilon = light.hardAngle - light.softAngle;
        float intensity = clamp((cosine - light.softAngle) / epsilon, 0.0f, 1.0f);
        float d = length(camPos - fragPos);
        float distScale = 1.0f / (light.k0k1k2.x + light.k0k1k2.y * d + light.k0k1k2.z * d * d);
        
        return light.color * light.diffuse * intensity * distScale;
    } else {
        return vec3(0.0f);
    }
}

void main() {
    vec3 resColor, lighting;
    vec3 objectColor = vec3(1.0f, 0.5f, 0.0f);
    vec3 viewDir = normalize(camPos - fragPos);

    lighting = CalcDirLight(dirLight, normalVec, viewDir);
    lighting += CalcPointLight(pointLight, normalVec, fragPos, viewDir);
    lighting += CalcConeLight(coneLight, viewDir);
        
    color = vec4(lighting * objectColor, 1.0f);
}

