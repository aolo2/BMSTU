#version 430 core

// do not importance sample the irradiance map

out vec4 fragColor;
in vec3 localPos;

uniform samplerCube environmentMap;

const float PI = 3.14159265359f;

void main() {
    vec3 N = normalize(localPos);
    vec3 color = vec3(0.0f);

    // go from world to tangent
    vec3 up = vec3(0.0f, 1.0f, 0.0f);
    vec3 right = cross(up, N);
    up = cross(N, right);
    
    int samples = 0;
    float angle_step = 0.025f;
    
    // hemishpere
    for (float phi = 0.0f; phi < 2.0f * PI; phi += angle_step) {
        for (float theta = 0.0f; theta < 0.5f * PI; theta += angle_step) {
            // spherical coordianates sample vector
            vec3 Tsample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            
            // world space sample vector
            vec3 Wsample = Tsample.x * right + Tsample.y * up + Tsample.z * N;
            
            // read from env map and (?) scale by both angles (/?)
            color += texture(environmentMap, Wsample).rgb * cos(theta) * sin(theta);
            samples++;
        }
    }    
    
    // normalize
    color = PI * color * (1.0f / float(samples));
    
    fragColor = vec4(color, 1.0f);
}
