#include "Shader.h"

Shader::Shader(const std::string &vert_path, const std::string &frag_path) {
    const char *vs_src, *fs_src;

    // Get source
    std::ifstream t1(vert_path);
    std::string vert_str((std::istreambuf_iterator<char>(t1)),
                    std::istreambuf_iterator<char>());

    std::ifstream t2(frag_path);
    std::string frag_str((std::istreambuf_iterator<char>(t2)),
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
        throw;
    }

    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw;
    }

    glAttachShader(this->Program, fs);
    glAttachShader(this->Program, vs);
    glLinkProgram(this->Program);

    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
}

void Shader::setUniform(const std::string &varString, const glm::mat4 &matrix) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string &varString, const glm::vec3 &vector) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniform3f(loc, vector.x, vector.y, vector.z);
}

void Shader::setUniform(const std::string &varString, float value) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniform1f(loc, value);
}

void Shader::setUniform1i(const std::string &varString, int value) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniform1i(loc, value);
}

void Shader::useProgram() {
    glUseProgram(this->Program);
}
