#ifndef _SHAPES_H
#define _SHAPES_H

#include <vector>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define PI 3.1415926535f

void gen_sphere(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector,
                float radius, int slices, int stacks);

void gen_cube(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector,
            float half_side);

void gen_quad(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector,
              float half_side);

void gen_random_shit(std::vector<glm::vec3> &vectices_vector, std::vector<int> &index_vector, int size);

#endif //_SHAPES_H
