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
        void useProgram() const;
        void setUniform(const std::string &varString, const glm::mat4 &matrix);
        void setUniform(const std::string &varString, const glm::vec3 &vector);
        void setUniform(const std::string &varString, float value);
        void setUniform1i(const std::string &varString, int value);
        void bindUniformBlock(const std::string &block);
        
        GLuint gProgram() const { return Program; }
            
        Shader(const std::string &vert_path, const std::string &frag_path);
        Shader(const Shader &other);
    private:
        Shader &operator=(Shader other);
        GLuint Program;
    };

}