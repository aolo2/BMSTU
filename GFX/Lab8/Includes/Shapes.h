#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace Utils {
    
    static const glm::vec3 X_POS = glm::vec3(1.0f, 0.0f, 0.0f);
    static const glm::vec3 Y_POS = glm::vec3(0.0f, 1.0f, 0.0f);
    static const glm::vec3 Z_POS = glm::vec3(0.0f, 0.0f, 1.0f);
    static const glm::vec3 X_NEG = glm::vec3(-1.0f, 0.0f, 0.0f);
    static const glm::vec3 Y_NEG = glm::vec3(0.0f, -1.0f, 0.0f);
    static const glm::vec3 Z_NEG = glm::vec3(0.0f, 0.0f, -1.0f);
    
    namespace Normals {
        std::vector<GLfloat> cube();
    }
    
    namespace Colors {
        std::vector<GLfloat> cube();
    }
        
    namespace Shapes {
        std::vector<GLfloat> cube();
        std::vector<GLfloat> sphere(unsigned int slices = 60);
    }
}