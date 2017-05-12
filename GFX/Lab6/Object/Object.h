#ifndef _OBJECT_H
#define _OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

#include "../Shapes/Shapes.h"

class Object {
public:
    Object(Shape shape, GLenum drawmode = GL_FILL);
    ~Object();
    void render();
    std::vector<glm::vec3> get_3d_collision();
private:
    Shape shape;
    std::vector<GLfloat> vertices;
    GLuint VAO, VBO;
    GLenum drawmode;
    unsigned int slices = 60;
};

#endif
