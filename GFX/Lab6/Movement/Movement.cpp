#include "Movement.h"

#include <iostream>

const float MOVEMENT_SPEED(0.5f);
glm::vec3 MOVEMENT_DIRECTION(0.5f, -0.2f, -1.0f);

Face calculate_collission(const std::vector<glm::vec3> &vertices) {
    for (const auto &vertex : vertices) {
        if (vertex.x >= 1.0f) {
            return Face::RIGHT;
        }
        
        if (vertex.x <= -1.0f) {
            return Face::LEFT;
        }
        
        if (vertex.y >= 1.0f) {
            return Face::TOP;
        }
        
        if (vertex.y <= -1.0f) {
            return Face::BOTTOM;
        }
        
        if (vertex.z >= 1.0f) {
            return Face::FRONT;
        }
        
        if (vertex.z <= -1.0f) {
            return Face::BACK;
        }
    }
    
    return Face::NONE;
}

glm::vec3 calculate_position(const glm::vec3 &old_pos, const std::vector<glm::vec3> &vertices, float dt) {

    Face face = calculate_collission(vertices);
    glm::vec3 normal;
    
    switch (face) {
        case Face::TOP:
            normal = glm::vec3(0.0f, -1.0f, 0.0f);
            break;
        case Face::FRONT:
            normal = glm::vec3(0.0f, 0.0f, -1.0f);
            break;
        case Face::RIGHT:
            normal = glm::vec3(-1.0f, 0.0f, 0.0f);
            break;
        case Face::BOTTOM:
            normal = glm::vec3(0.0f, 1.0f, 0.0f);
            break;
        case Face::BACK:
            normal = glm::vec3(0.0f, 0.0f, 1.0f);
            break;
        case Face::LEFT:
            normal = glm::vec3(1.0f, 0.0f, 0.0f);
            break;
        default:
            break;
    }
    
    if (face != Face::NONE) {
        MOVEMENT_DIRECTION = glm::reflect(MOVEMENT_DIRECTION, normal);
        // std::cout << face << std::endl;
    }
    
    return old_pos + MOVEMENT_DIRECTION * MOVEMENT_SPEED * dt;
}
