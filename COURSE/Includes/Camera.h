#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace utils {

const GLfloat YAW = 0.0f;
const GLfloat PITCH = -25.0f;
const GLfloat SPEED = 0.25f;
const GLfloat SENSITIVITY = 0.1f;

class camera {
public:
    glm::mat4 view_matrix();
    camera(glm::vec3 pos, glm::vec3 up);

    camera(const camera& other) = default;
    camera& operator=(camera other) = delete;

    void process_cursor(GLfloat x_offset, GLfloat y_offset, GLboolean cap_pitch);
    void process_movement(GLint key);

private:
    void update_angles();

    glm::vec3 pos, front, up, right, world_up = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat yaw = YAW, pitch = PITCH;
    GLfloat movement_speed = SPEED, mouse_sensitivity = SENSITIVITY;
};
}