#include "../Includes/Camera.h"

#include <GLFW/glfw3.h>

namespace utils {

camera::camera(glm::vec3 pos, glm::vec3 up)
{
    this->pos = pos;
    this->world_up = normalize(up);

    this->pitch = PITCH;
    this->yaw = YAW;

    this->update_angles();
}

glm::mat4 camera::view_matrix()
{
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

void camera::process_movement(GLint key)
{
    switch (key) {
    case GLFW_KEY_W:
        this->pos += this->front * movement_speed;
        break;
    case GLFW_KEY_A:
        this->pos -= this->right * movement_speed;
        break;
    case GLFW_KEY_S:
        this->pos -= this->front * movement_speed;
        break;
    case GLFW_KEY_D:
        this->pos += this->right * movement_speed;
        break;
    default:
        break;
    }

    this->update_angles();
}

void camera::process_cursor(GLfloat x_offset, GLfloat y_offset, GLboolean cap_pitch)
{
    x_offset *= this->mouse_sensitivity;
    y_offset *= this->mouse_sensitivity;

    this->yaw += x_offset;
    this->pitch += y_offset;

    if (cap_pitch) {
        if (this->pitch > 89.0f) {
            this->pitch = 89.0f;
        }
        if (this->pitch < -89.0f) {
            this->pitch = -89.0f;
        }
    }

    this->update_angles();
}

void camera::update_angles()
{
    glm::vec3 new_front;

    new_front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    new_front.y = sin(glm::radians(this->pitch));
    new_front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(new_front);
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
}