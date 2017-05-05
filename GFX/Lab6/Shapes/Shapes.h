#ifndef _SHAPES_H
#define _SHAPES_H


#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

const float PI = 3.1415926535f;

// GLM (maths)
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Shape {
    CUBE,
    SPHERE,
    QUAD,
    SPIN_SURFACE,
};

std::vector<GLfloat> gen_cube();
std::vector<GLfloat> gen_quad();
std::vector<GLfloat> gen_surface(unsigned int slices, const std::vector<glm::vec2> &broken_line);

#endif // Shapes.h
