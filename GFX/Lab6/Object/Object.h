#ifndef _OBJECT_H
#define _OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

#include "../Shapes/Shapes.h"

class Object {
public:
    Object(Shape shape);
    ~Object();
    void render();
private:
    Shape shape;
    std::vector<GLfloat> vertices;
    GLuint VAO, VBO;
};

#endif
