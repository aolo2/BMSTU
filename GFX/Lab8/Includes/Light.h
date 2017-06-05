#pragma once

namespace Utils {

enum LightType {
    DIR,
    SPOT,
    CONE,
    SPHERE,
    MESH,
};

class Light {
public:
    Light(const glm::vec3 &color, const glm::vec3 &position, LightType type)
    : color(color),
    position(position),
    type(type) {}

    glm::vec3 gcolor() const { return color; }
    glm::vec3 gposition() const { return position; }
    glm::vec3 gtype() const { return type; }
    
private:
    glm::vec3 color;        // 16
    glm::vec3 position;     // 16
    LightType type;         // 4
}

}