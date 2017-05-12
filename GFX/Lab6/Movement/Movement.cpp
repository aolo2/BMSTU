#include "Movement.h"

#include <iostream>

float MOVEMENT_SPEED(0.8f);
glm::vec3 MOVEMENT_DIRECTION(1.0f);

bool outside = false, inside_safe = true;

const glm::vec3 normals[6] = {
    glm::vec3(0.0f, -1.0f, 0.0f), // top
    glm::vec3(0.0f, 0.0f, -1.0f), // front
    glm::vec3(-1.0f, 0.0f, 0.0f), // right
    glm::vec3(0.0f, 1.0f, 0.0f), // bottom
    glm::vec3(0.0f, 0.0f, 1.0f), // back
    glm::vec3(1.0f, 0.0f, 0.0f), // left
};

void set_start_dir(const glm::vec3 &start_dir) {
    MOVEMENT_DIRECTION = start_dir;
}

void set_speed(float speed) {
    MOVEMENT_SPEED = speed;
}

glm::vec3 get_current_dir() {
    return MOVEMENT_DIRECTION;
}

glm::vec3 calculate_position(const glm::vec3 &old_pos, float radius, float dt) {
    if (!outside && (abs(old_pos.x) >= 1.0f || abs(old_pos.y) >= 1.0f|| abs(old_pos.z) >= 1.0f)) {
        outside = true;
        if (old_pos.x >= 1.0f) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::RIGHT]);
        } else if (old_pos.x <= -1.0f) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::LEFT]);
        }
        
        if (old_pos.y >= 1.0f) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::TOP]);
        } else if (old_pos.y <= -1.0f) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::BOTTOM]);
        }
        
        if (old_pos.z >= 1.0f) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::FRONT]);
        } else if (old_pos.z <= -1.0f) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::BACK]);
        }
        
    } else if (inside_safe && ((abs(old_pos.x - 1.0f) <= radius) || (abs(old_pos.x + 1.0f) <= radius) ||
               (abs(old_pos.y - 1.0f) <= radius) || (abs(old_pos.y + 1.0f) <= radius) ||
               (abs(old_pos.z - 1.0f) <= radius) || (abs(old_pos.z + 1.0f) <= radius))) {
        outside = false;
        inside_safe = false;
        
        if ((abs(old_pos.x - 1.0f) <= radius)) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::RIGHT]);
        } else if (abs(old_pos.x + 1.0f) <= radius) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::LEFT]);
        }
        
        if (abs(old_pos.y - 1.0f) <= radius) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::TOP]);
        } else if (abs(old_pos.y + 1.0f) <= radius) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::BOTTOM]);
        }
        
        if (abs(old_pos.z - 1.0f) <= radius) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::FRONT]);
        } else if (abs(old_pos.z + 1.0f) <= radius) {
            MOVEMENT_DIRECTION = reflect(MOVEMENT_DIRECTION, normals[Face::BACK]);
        }
        
    } else {
        outside = false;
        inside_safe = true;
    }
                
    return old_pos + glm::normalize(MOVEMENT_DIRECTION) * MOVEMENT_SPEED * dt;
}
