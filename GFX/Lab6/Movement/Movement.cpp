#include "Movement.h"

#include <iostream>

float LIGHT_SPEED(0.5f), MOVEMENT_SPEED(0.8f);
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

void set_light_speed(float light_speed) {
    LIGHT_SPEED = light_speed;
}

glm::vec3 get_current_dir() {
    return MOVEMENT_DIRECTION;
}

float get_light_speed() {
    return LIGHT_SPEED;
}

glm::vec3 calculate_light_positon(const glm::vec3 &old_light_pos, const bool * const keys, float dt) {
    glm::vec3 dir(0.0f);
    
    if (keys[GLFW_KEY_UP]) {
        if (keys[1001] & GLFW_MOD_SHIFT) {
            dir -= normals[Face::BACK];
        } else {
            dir += normals[Face::BOTTOM];
        }
    }
    
    if (keys[GLFW_KEY_DOWN]) {
        if (keys[1001] & GLFW_MOD_SHIFT) {
            dir += normals[Face::BACK];   
        } else {
            dir -= normals[Face::BOTTOM];
        }
    }
    
    if (keys[GLFW_KEY_LEFT]) {
        dir += normals[Face::RIGHT];   
    }
    
    if (keys[GLFW_KEY_RIGHT]) {
        dir += normals[Face::LEFT];      
    }
    
    if (dir == glm::vec3(0.0f)) {
	return old_light_pos;
    }

    return old_light_pos + glm::normalize(dir) * LIGHT_SPEED * dt;
}

glm::vec3 calculate_position(const glm::vec3 &old_pos, float radius, float dt) {
    using glm::reflect;
    using glm::abs;
    
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
