#include "Shader.h"

Shader::Shader(const std::string &vert_path, const std::string &frag_path) {
    const char *vs_src, *fs_src;
    
    // Get source
    std::ifstream t;
    
    t = std::ifstream(vert_path);
    std::string vert_str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
                    
    t = std::ifstream(frag_path);
    std::string frag_str((std::istreambuf_iterator<char>(t)),
                    std::istreambuf_iterator<char>());
                    
    vs_src = vert_str.c_str();
    fs_src = frag_str.c_str();


    this->Program = glCreateProgram();

    // Compile and link    
    GLuint vs, fs;
    GLint success;
    
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src, NULL);
    glCompileShader(vs);
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    glAttachShader(this->Program, fs);
    glAttachShader(this->Program, vs);
    glLinkProgram(this->Program);
    
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::useProgram() {
    glUseProgram(this->Program);
}

GLuint Shader::getProgram() {
    return this->Program;
}