#ifndef _SHADER_H
#define _SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Lights.h"

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream> 


class Shader {
    public:
        void useProgram();
        
        GLuint getProgram();
        
        Shader(const std::string &vert_path, const std::string &frag_path);
        
        void setUniform(const std::string &varString, const glm::mat4 &matrix);
        
        void setUniform(const std::string &varString, const glm::vec3 &vector);
        
        void setUniform(const std::string &varString, const DirLight &light);
        
        void setUniform(const std::string &varString, const PointLight &light);
        
        void setUniform(const std::string &varString, const ConeLight &light);
        
        void setUniform(const std::string &varString, float value);
    private:
        GLuint Program;
        GLchar infoLog[512];
};

#endif // _SHADER_H

