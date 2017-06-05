#pragma once

#include "Shader.h"
#include "Object.h"
#include "Material.h"
#include "Light.h"
#include "Camera.h"

#include <map>
#include <set>

namespace Utils {

enum { UNDEFINED = 10000000u; }
typedef ID unsigned int;    

class Scene {
public:
    Scene();
    
    ID add(const Object &object);
    ID add(const Camera &camera);
    ID add(const Light &light);
    ID add(const Material &material);
    
    void removeLight(ID id);
    void removeObject(ID id);
    void removeCamera(ID id);
    void removeMaterial(ID id);
    
    void render() const;

private:
    /* Only one scene for now */
    Scene(const &Scene other);
    Scene &operator=(Scene other);
    
    ID getID();
    
    std::map<ID, Object> objects;
    std::map<ID, Material> materials;
    std::map<ID, Light> lights;
    std::map<ID, Camera> cameras;
    
    std::set<ID> activeObjects;
    // std::set<ID> activeMaterials;
    std::set<ID> activeLights;

    GLuint lightsUBO, camerasUBO;

    ID activeCamera;
    ID counter;
}
    
    
}