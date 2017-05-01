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

void Shader::setUniform(const std::string &varString, float value) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniform1f(loc, value);
}

void Shader::setUniform(const std::string &varString, const glm::mat4 &matrix) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setUniform(const std::string &varString, const glm::vec3 &vector) {
    GLint loc = glGetUniformLocation(this->Program, varString.c_str());
    glUniform3f(loc, vector.x, vector.y, vector.z);
}

void Shader::setUniform(const std::string &varString, const DirLight &light) {
    GLuint Program = this->Program;

    glUniform1f(glGetUniformLocation(Program, (varString + ".ambient").c_str()), light.ambient);
    glUniform1f(glGetUniformLocation(Program, (varString + ".diffuse").c_str()), light.diffuse);
    glUniform1f(glGetUniformLocation(Program, (varString + ".specular").c_str()), light.specular);
    
    glUniform3f(glGetUniformLocation(Program, (varString + ".direction").c_str()), light.direction.x, light.direction.y, light.direction.z);
    glUniform3f(glGetUniformLocation(Program, (varString + ".color").c_str()), light.color.x, light.color.y, light.color.z);
}

void Shader::setUniform(const std::string &varString, const PointLight &light) {
    GLuint Program = this->Program;

    glUniform1f(glGetUniformLocation(Program, (varString + ".ambient").c_str()), light.ambient);
    glUniform1f(glGetUniformLocation(Program, (varString + ".diffuse").c_str()), light.diffuse);
    glUniform1f(glGetUniformLocation(Program, (varString + ".specular").c_str()), light.specular);
    
    glUniform3f(glGetUniformLocation(Program, (varString + ".position").c_str()), light.position.x, light.position.y, light.position.z);
    glUniform3f(glGetUniformLocation(Program, (varString + ".color").c_str()), light.color.x, light.color.y, light.color.z);
    
    glUniform1f(glGetUniformLocation(Program, (varString + ".k0").c_str()), light.k0);
    glUniform1f(glGetUniformLocation(Program, (varString + ".k1").c_str()), light.k1);
    glUniform1f(glGetUniformLocation(Program, (varString + ".k2").c_str()), light.k2);
}

void Shader::setUniform(const std::string &varString, const ConeLight &light) {
    GLuint Program = this->Program;
    
    glUniform1f(glGetUniformLocation(Program, (varString + ".ambient").c_str()), light.ambient);
    glUniform1f(glGetUniformLocation(Program, (varString + ".diffuse").c_str()), light.diffuse);
    glUniform1f(glGetUniformLocation(Program, (varString + ".specular").c_str()), light.specular);
    
    
    glUniform3f(glGetUniformLocation(Program, (varString + ".direction").c_str()), light.direction.x, light.direction.y, light.direction.z);
    glUniform3f(glGetUniformLocation(Program, (varString + ".color").c_str()), light.color.x, light.color.y, light.color.z);
    
    glUniform1f(glGetUniformLocation(Program, (varString + ".k0").c_str()), light.k0);
    glUniform1f(glGetUniformLocation(Program, (varString + ".k1").c_str()), light.k1);
    glUniform1f(glGetUniformLocation(Program, (varString + ".k2").c_str()), light.k2);
    
    glUniform1f(glGetUniformLocation(Program, (varString + ".hardCutOff").c_str()), light.hardCutOff);
    glUniform1f(glGetUniformLocation(Program, (varString + ".softCutOff").c_str()), light.softCutOff);
}

void Shader::useProgram() {
    glUseProgram(this->Program);
}

GLuint Shader::getProgram() {
    return this->Program;
}