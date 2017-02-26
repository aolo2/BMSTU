#ifndef _CAMERA_H
#define _CAMERA_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 0.5f;
const GLfloat SENSITIVITY = 0.1f;

class Camera {
public:
    Camera(glm::vec3 pos, glm::vec3 up);
    Camera() : Camera(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {}
    glm::mat4 viewMatrix();
    glm::vec3 getPosition();
    glm::vec3 getFront();
    void processCursor(GLfloat x_offset, GLfloat y_offset, GLboolean);
    void processMovement(GLint key);
private:
    glm::vec3 pos, front, up, right, worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    GLfloat yaw = YAW, pitch = PITCH;
    GLfloat movementSpeed = SPEED, mouseSensitivity = SENSITIVITY;
    void updateAngles();
};

#endif
