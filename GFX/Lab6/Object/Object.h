#ifndef _OBJECT_H
#define _OBJECT_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>
#include <string>
#include <SOIL/SOIL.h>

#include "../Shapes/Shapes.h"

class Object {
public:
    Object(Shape shape, GLenum drawmode = GL_FILL);
    ~Object();
    void render();
    float get_collision_radius();
private:
    void load_texture(const std::string &path);
    
    Shape shape;
    std::vector<GLfloat> vertices;
    GLuint VAO, VBO, albedo;
    GLenum drawmode;
    unsigned int slices = 60, broken_points;
    float collision_radius;
};

#endif
