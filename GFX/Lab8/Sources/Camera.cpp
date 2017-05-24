#include "../Includes/Camera.h"

#include <GLFW/glfw3.h>

using namespace Utils;

Camera::Camera(glm::vec3 pos, glm::vec3 up) {
    this->pos = pos;
    this->worldUp = normalize(up);

    this->pitch = PITCH;
    this->yaw = YAW;

    this->updateAngles();
}

glm::mat4 Camera::viewMatrix() {
    return glm::lookAt(this->pos, this->pos + this->front, this->up);
}

glm::vec3 Camera::getPosition() {
    return this->pos;
}

glm::vec3 Camera::getFront() {
    return this->front;
}

void Camera::processMovement(GLint key) {
    switch (key) {
        case GLFW_KEY_W:
            this->pos += this->front * movementSpeed;
            break;
        case GLFW_KEY_A:
            this->pos -= this->right * movementSpeed;
            break;
        case GLFW_KEY_S:
            this->pos -= this->front * movementSpeed;
            break;
        case GLFW_KEY_D:
            this->pos += this->right * movementSpeed;
            break;
    }

    this->updateAngles();
}

void Camera::processCursor(GLfloat x_offset, GLfloat y_offset, GLboolean capPitch) {
    x_offset *= this->mouseSensitivity;
    y_offset *= this->mouseSensitivity;

    this->yaw += x_offset;
    this->pitch += y_offset;

    if (capPitch) {
        if (this->pitch > 89.0f) { this->pitch = 89.0f; }
        if (this->pitch < -89.0f) { this->pitch = -89.0f; }
    }

    this->updateAngles();
}

void Camera::updateAngles() {
    glm::vec3 newFront;

    newFront.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    newFront.y = sin(glm::radians(this->pitch));
    newFront.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(newFront);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}
