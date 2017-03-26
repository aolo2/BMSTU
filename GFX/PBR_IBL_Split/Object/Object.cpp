#include "Object.h"

#include <iostream>

Object::Object(GLuint VAO, Shape shape) {
    int slices = 48, stacks = 48;

    glBindVertexArray(VAO);

    switch (shape) {
        case Shape::SPHERE:
            vertices = gen_sphere(slices, stacks);
            this->dots = slices * (stacks + 1) * 18;
            break;
        case Shape::CUBE:
            vertices = gen_cube();
            this->dots = 216;
            break;
        case Shape::QUAD:
            vertices = gen_quad();
            this->dots = 12;
            break;
    }

    this->shape = shape;

    glGenBuffers(1, &(this->VBO));
    glBindVertexArray(0);
}

void Object::setModelMatrix(const glm::mat4 &model) {
    this->model = model;
}

const glm::mat4 &Object::getModelMatrix() {
    return this->model;
}

void Object::render() {
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, dots * sizeof(GLfloat), this->vertices, GL_STATIC_DRAW);

    switch (this->shape) {
        case Shape::SPHERE:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(1);
            break;
        case Shape::CUBE:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            break;
        case Shape::QUAD:
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            break;
    }

    glDrawArrays(GL_TRIANGLES, 0, dots);
}

Object::~Object() {
    glDeleteBuffers(1, &(this->VBO));
    delete[] vertices;
}
