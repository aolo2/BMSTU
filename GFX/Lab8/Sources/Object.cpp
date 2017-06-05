#include "../Includes/Object.h"

namespace Utils {

Object::Object(Shape shape, Utils::ID materialID) : materialID(materialID) {
    if (shape == Shape::CUBE) {
        vertices = Shapes::cube();
        normals = Normals::cube();
    } else if (shape == Shape::QUAD) {
        vertices = Shapes::quad();
        normals = Normals::quad();
    } else if (shape == Shape::CUBE_GRAY) {
        vertices = Shapes::cube();
        normals = Normals::cube();
    }
    
    init();
}

void Object::render() const {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, (vertices.size() + normals.size()) / 3);
    glBindVertexArray(0);
}

Object::Object(const Object &other) 
    : vertices(other.gvertices()),
    normals(other.gnormals()),
    shader(other.gshader()) 

{ init(); }

void Object::init() {
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(GLfloat), (GLvoid *) 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), normals.size() * sizeof(GLfloat), normals.data());
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) (vertices.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Object::~Object() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

}