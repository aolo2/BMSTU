#ifndef _SHAPES_H
#define _SHAPES_H

#define PI 3.1415926

#include <vector>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void gen_sphere(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector, 
                float radius, int slices, int stacks);

void gen_cube(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector, float half_side);

#endif