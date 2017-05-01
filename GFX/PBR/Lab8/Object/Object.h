#ifndef _OBJECT_H
#define _OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

#include "../Shapes/Shapes.h"

class Object {
public:
    Object(GLuint VAO, Shape shape);
    virtual ~Object();
    void render();
    void setModelMatrix(const glm::mat4 &model);
    const glm::mat4 &getModelMatrix();
private:
    bool first = true;
    Shape shape;
    GLuint VBO;
    GLfloat *vertices;
    GLint dots;
    glm::mat4 model;
};

#endif
