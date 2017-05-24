#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

namespace Utils {
    
    class Shader {
    public:
        void useProgram();
        void setUniform(const std::string &varString, const glm::mat4 &matrix);
        void setUniform(const std::string &varString, const glm::vec3 &vector);
        void setUniform(const std::string &varString, float value);
        void setUniform1i(const std::string &varString, int value);
            
        Shader(const std::string &vert_path, const std::string &frag_path);
    private:
        Shader(const Shader &other);
        Shader &operator=(Shader other);
        GLuint Program;
        GLchar infoLog[512];
    };

}