#include "../Includes/Object.h"

using namespace Utils;

Object::Object() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    vertices = Shapes::cube();
    
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Object::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
    glBindVertexArray(0);
}

Object::Object(const Object &other) {
    vertices = other.gvertices();
    VAO = other.gVAO();
    VBO = other.gVBO();
}

void Object::swap(Object &other) {
    
}

Object &Object::operator=(Object other) {
    swap(other);
    
    return *this;
}

Object::~Object() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}