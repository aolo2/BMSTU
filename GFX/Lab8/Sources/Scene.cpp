#include "../Includes/Scene.h"

namespace Utils {

Scene::Scene() {
    activeCamera = UNDEFINED;
    counter = 0u;
    
    glGenBuffers(1, &lightsUBO);
    glGenBuffers(1, &camerasUBO);
    
    std::size_t lights_memory = activeLights.size() * 36; // 16 + 16 + 4
    
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    glBufferData(GL_UNIFORM_BUFFER, 360, NULL, GL_STATIC_DRAW); // 360 - tmp
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
        
    for (const auto &material : materials) {
        material.gshader().bindUniformBlock("ActiveLights", 0);
    }
    
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboExampleBlock);
    
    // todo: fix
    std::vector<Light> lights_data;
    for (auto lightID : activeLights) {
        lights_data.push_back(lights[lightID]);
    }    
    
    glBindBuffer(GL_UNIFORM_BUFFER, lightsUBO);
    for (unsigned int i = 0; i < lights_data.size(); i++) {
        glBufferSubData(GL_UNIFORM_BUFFER, i * 36, &lights_data[i]);
    }
    
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
      
}

void Scene::render() const {
    // pass active camera and light to UBO
    
    for (auto id : activeObjects) {
        materials[objects[id].gmaterialID()].useProgram();
        objets[id].render();
        // pass model matrix to shader sometime
        // mb in the object itself (it has pointer to shader)
    }
}

ID getID() {
    return ++counter - 1; 
}

ID Scene::add(const Object &object) {
    ID id = getID();
    objects[id] = object;
    
    return id;
}

ID Scene::add(const Camera &camera) {
    ID id = getID();
    cameras[id] = object;
    
    return id;
}

ID Scene::add(const Light &light) {
    ID id = getID();
    lights[id] = object;
    
    return id;
}

ID Scene::add(const Material &material) {
    ID id = getID();
    materials[id] = object;
    
    return id;
}

void Scene::removeObject(ID id) {
    activeObjects.erase(id);
    objects.erase(id);
}

void Scene::removeMaterial(ID id) {
    materials.erase(id);
}

void Scene::removeCamera(ID id) {
    cameras.erase(id);
    
    if (id == activeCamera) {
        activeCamera = UNDEFINED;    
    }
}

void Scene::removeLight(ID id) {
    activeLights.erase(id);
    lights.erase(id);
}
    
}