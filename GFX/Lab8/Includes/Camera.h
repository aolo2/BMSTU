#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Utils {

    const GLfloat YAW = 0.0f;
    const GLfloat PITCH = -25.0f;
    const GLfloat SPEED = 0.25f;
    const GLfloat SENSITIVITY = 0.1f;
    
    class Camera {
    public:
        Camera(glm::vec3 pos, glm::vec3 up);
        Camera() : Camera(glm::vec3(-3.0f, 2.0f, 0.75f), glm::vec3(0.0f, 1.0f, 0.0f)) {}
        glm::mat4 viewMatrix();
        glm::vec3 getPosition();
        glm::vec3 getFront();
        void processCursor(GLfloat x_offset, GLfloat y_offset, GLboolean capPitch);
        void processMovement(GLint key);
    private:
        glm::vec3 pos, front, up, right, worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        GLfloat yaw = YAW, pitch = PITCH;
        GLfloat movementSpeed = SPEED, mouseSensitivity = SENSITIVITY;
        void updateAngles();
    };

}