#ifndef _SHAPES_H
#define _SHAPES_H


#define GLEW_STATIC
#include <GL/glew.h>

#define PI 3.1415926535f

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum Shape {
    CUBE,
    SPHERE,
    QUAD,
};

GLfloat *gen_cube();
GLfloat *gen_quad();
GLfloat *gen_surface(unsigned int slices);
GLfloat *gen_sphere(int slices, int stacks);

#endif // Shapes.h
