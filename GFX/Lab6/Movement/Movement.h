#ifndef _MOVEMENT_H
#define _MOVEMENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum Face {
    TOP = 0,
    FRONT = 1,
    RIGHT = 2,
    BOTTOM = 3,
    BACK = 4,
    LEFT = 5,
};

void set_start_dir(const glm::vec3 &start_dir);
void set_speed(float speed);
glm::vec3 get_current_dir();

glm::vec3 calculate_position(const glm::vec3 &old_pos, float radius, float dt);

#endif // _MOVEMENT_H