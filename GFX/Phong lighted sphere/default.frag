#version 430 core

out vec4 color;

in vec3 normalVec;
in vec3 fragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
    float ambient = 0.03f;
    float specularStrength = 0.5f;
    
    vec3 ambientLight = lightColor * ambient;
    vec3 norm = normalize(normalVec);
    vec3 lightDir = normalize(lightPos - fragPos);  
    
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 reflectDir = normalize(reflect(-lightDir, norm));
    
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    float d= distance(lightPos, fragPos);
    float k0 = 0.0f, k1 = 0.2f, k2 = 0.8f;
    float distLight = 1 / (k0 * d * d + k1 * d + k2);
    
    color = vec4(objectColor * distLight * (ambientLight + diffuse + specular), 1.0f);
}