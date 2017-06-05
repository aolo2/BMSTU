#pragma once

#include <glm/glm.hpp>

namespace Utils {



class Material {
public:
    Material(const Shader &shader, const glm::vec3 &a, const glm::vec3 &d, const glm::vec3 &s)
    : shader(shader),
    ambient(a),
    diffuse(d),
    specular(s) {}
    
    Shader gshader() const { return shader; }
    glm::vec3 gsambient() const { return ambient; }
    glm::vec3 gdiffuse() const { return diffuse; }
    glm::vec3 gspecular() const { return specular; }
    
    void useProgram() const { shader.useProgram(); }
    
private:
    Shader shader;
    const glm::vec3 ambient;
    const glm::vec3 diffuse;
    const glm::vec3 specular;
}

}