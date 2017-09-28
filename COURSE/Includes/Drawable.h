#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <string>
#include <vector>

namespace utils {

class drawable {
public:
    void render() const;
    drawable(const std::string& mesh_path);
    virtual ~drawable();
    drawable& operator=(drawable other) = delete;

private:
    void set_buffers();
    void load_obj(const std::string& path);

    GLenum drawmode;
    GLuint VAO, VBO;
    unsigned int dimension;

    std::vector<GLfloat> vertices, normals;
    std::vector<GLfloat> textures, colors;
};
}