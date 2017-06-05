#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

namespace Utils {

    const GLfloat YAW = 0.0f;
    const GLfloat PITCH = -25.0f;
    const GLfloat SPEED = 0.25f;
    const GLfloat SENSITIVITY = 0.1f;
    
    class Camera {
    public:
        Camera(glm::vec3 pos, glm::vec3 up);
        Camera() : Camera(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) {}
        
        glm::vec3 getPosition();
        glm::vec3 getFront();
        glm::mat4 gview() const { return viewMatrix(); }
        glm::mat4 gproj() const { return proj; }
        
        void processCursor(GLfloat x_offset, GLfloat y_offset, GLboolean capPitch);
        void processMovement(GLint key);
    private:
        glm::mat4 viewMatrix();
        void updateAngles();
        
        glm::vec3 pos, front, up, right;
        glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
        GLfloat yaw = YAW, pitch = PITCH;
        GLfloat movementSpeed = SPEED, mouseSensitivity = SENSITIVITY;
        const glm::mat4 proj;
    };

}