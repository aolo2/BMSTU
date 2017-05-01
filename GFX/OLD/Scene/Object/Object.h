#ifndef _OBJECT_H
#define _OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <SOIL/SOIL.h>

#include <vector>

#include "../Shapes/Shapes.h"

class Object {
public:
    Object(Shape shape, GLuint VAO);
    virtual ~Object();
    void render();
    void setModelMatrix(const glm::mat4 &model);
    const glm::mat4 &getModelMatrix();
private:
    bool first = true;
    GLuint VBO, diffuseMap, specularMap, normalMap;
    GLfloat *vertices;
    GLint dots;
    glm::mat4 model;
};

#endif
