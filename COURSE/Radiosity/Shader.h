#ifndef SCENE_SHADER_H
#define SCENE_SHADER_H

#define GLEW_STATIC

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

        void setUniform(const std::string &varString, const glm::vec2 &vector);

        void setUniform(const std::string &varString, float value);

        void setUniform1i(const std::string &varString, int value);

        Shader(const std::string &vert_path, const std::string &frag_path);

        Shader(const std::string &vert_path, const std::string &geom_path, const std::string &frag_path);

        Shader &operator=(Shader other) = delete;

    private:

        Shader(const Shader &other);

        GLuint Program;
    };

}

#endif //SCENE_SHADER_H
