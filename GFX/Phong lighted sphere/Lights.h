#ifndef _LIGHTING_H
#define _LIGHTING_H

struct DirLight {
    glm::vec3 direction;
    glm::vec3 color;

    GLfloat ambient, diffuse, specular;
    
    DirLight() : direction(0.0f, -3.0f, -1.0f), color(1.0f), ambient(0.1f), 
                diffuse(0.2f), specular(0.0f) { }
};  

struct PointLight {
    glm::vec3 position;
    glm::vec3 color;
    
    GLfloat ambient, diffuse, specular;
    GLfloat k0, k1, k2;
    
    PointLight() : position(1.0f), color(1.0f), ambient(0.1f), diffuse(0.8f), 
                specular(0.6f), k0(1.0f), k1(0.05f), k2(0.02f) { }
};

struct ConeLight {
    glm::vec3 direction;
    glm::vec3 color;
    
    GLfloat k0, k1, k2;
    GLfloat ambient, diffuse, specular;
    GLfloat hardCutOff, softCutOff;
    
    ConeLight() : color(1.0f), ambient(0.1f), diffuse(0.9f), 
                specular(0.9f), k0(1.0f), k1(0.3), k2(0.1), hardCutOff(cos(glm::radians(15.5f))), 
                softCutOff(cos(glm::radians(19.0f))) { }
};

#endif