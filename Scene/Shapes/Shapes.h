#ifndef _SHAPES_H
#define _SHAPES_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.1415926535f

enum Shape {
    LINE,
    QUAD,
    CUBE,
    SPHERE
};

void gen_sphere(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector,
                float radius, int slices, int stacks);

GLfloat *gen_cube(GLint *dots);

#endif //_SHAPES_H
