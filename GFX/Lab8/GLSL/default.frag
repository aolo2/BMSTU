#version 430 core

layout (location = 0) out vec4 color;

struct Light {
    vec3 color;
    vec3 position;
    uint type;
}

layout (std140) uniform ActiveLights {
    Light lights[10];    
}; 

void main() {
    color = vec4(1.0f);
}