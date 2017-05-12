#ifndef _MOVEMENT_H
#define _MOVEMENT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

enum Face {
    NONE,
    TOP,
    FRONT,
    RIGHT,
    BOTTOM,
    BACK,
    LEFT,
};

Face calculate_collission(const std::vector<glm::vec3> &vertices);

glm::vec3 calculate_position(const glm::vec3 &old_pos, const std::vector<glm::vec3> &vertices, float dt);

#endif // _MOVEMENT_H