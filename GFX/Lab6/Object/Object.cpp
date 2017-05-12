#include "Object.h"

#include <iostream>

Object::Object(Shape shape, GLenum drawmode) : shape(shape), VAO(0), 
               VBO(0), drawmode(drawmode) {
    
    std::vector<glm::vec2> broken_line;
    broken_line.emplace_back(0.2f, -0.8f);
    broken_line.emplace_back(0.3f, -0.6f);
    broken_line.emplace_back(0.4f, -0.4f);
    broken_line.emplace_back(0.3f, -0.2f);
    broken_line.emplace_back(0.2f, 0.0f);
    broken_line.emplace_back(0.3f, 0.2f);
    broken_line.emplace_back(0.4f, 0.4f);
    broken_line.emplace_back(0.3f, 0.6f);
    broken_line.emplace_back(0.2f, 0.8f);
    broken_line.emplace_back(0.3f, 1.0f);
    broken_line.emplace_back(0.4f, 1.2f);
    
    switch (shape) {
        case Shape::SPIN_SURFACE:
            vertices = gen_surface(this->slices, broken_line);
            break;
        case Shape::CUBE:
            vertices = gen_cube();
            break;
        case Shape::QUAD:
            vertices = gen_quad();
            break;
    }
    
    glGenVertexArrays(1, &(this->VAO));
    glGenBuffers(1, &(this->VBO));

    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    
    switch (this->shape) {
        case Shape::SPIN_SURFACE:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            break;
        case Shape::CUBE:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            break;
        case Shape::QUAD:
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            break;
    }
    
    glBindVertexArray(0);
}

std::vector<glm::vec3> Object::get_3d_collision() {
    std::vector<glm::vec3> res;
    for (int i = 0; i < vertices.size() - 5; i += 6) {
        res.push_back(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]));
    }
    
    return res;
}

void Object::render() {
    glBindVertexArray(this->VAO);
    
    glPolygonMode(GL_FRONT_AND_BACK, drawmode);

    switch (this->shape) {
        case Shape::SPIN_SURFACE:
            glDrawArrays(GL_TRIANGLE_FAN, 0, (slices + 1));
            glDrawArrays(GL_TRIANGLE_STRIP, (slices + 1), (slices + 1) * 20);
            glDrawArrays(GL_TRIANGLE_FAN, (slices + 1) * 21, (slices + 1));
            break;
        case Shape::CUBE:
            glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);
            break;
        case Shape::QUAD:
            glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 2);
            break;
        
    }
    
    glBindVertexArray(0);
}

Object::~Object() {
    glDeleteBuffers(1, &(this->VBO));
    glDeleteVertexArrays(1, &(this->VAO));
}