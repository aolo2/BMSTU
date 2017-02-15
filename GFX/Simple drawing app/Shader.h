#ifndef _SHADER_H
#define _SHADER_H

#define GLEW_STATIC

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream> 

#include <GL/glew.h>

class Shader {
    public:
        Shader (const std::string &vert_path, const std::string &frag_path);
        void useProgram();
        GLuint getProgram();
    private:
        GLuint Program;
        GLchar infoLog[512];
};

#endif // _SHADER_H