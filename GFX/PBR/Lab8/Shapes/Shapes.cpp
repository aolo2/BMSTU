#include "Shapes.h"
#include <iostream>
#include <vector>

GLfloat *gen_quad() {
    return new GLfloat[12] {
        -1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, 1.0f,

        -1.0f, -1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f,
    };
}

GLfloat *gen_sphere(int slices, int stacks) {

    std::vector<glm::vec3> vertices_vector;
    std::vector<int> index_vector;

    for (int i = 0; i <= stacks; i++) {
        float V = i / (float) stacks;
        float phi = V * PI;

        // Loop Through Slices
        for (int j = 0; j <= slices; j++) {
            float U = (float) j / (float) slices;
            float theta = U * 2 * PI;

            // Calc The Vertex Positions
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            // Push Back Vertex Data
            vertices_vector.push_back(glm::vec3(x, y, z) * 0.5f);
        }
    }


    // Calc The Index Positions
    for (int i = 0; i < slices * stacks + slices; i++){
        index_vector.push_back(i);
        index_vector.push_back(i + slices + 1);
        index_vector.push_back(i + slices);

        index_vector.push_back(i + slices + 1);
        index_vector.push_back(i);
        index_vector.push_back(i + 1);
    }

    glm::vec3 tmp;

    GLfloat *res = new GLfloat[index_vector.size() * 3];
    for (int i = 0, j = 0; i < index_vector.size(); i++, j+= 3) {

        tmp = vertices_vector[index_vector[i]];

        res[j] = tmp.x;
        res[j + 1] = tmp.y;
        res[j + 2] = tmp.z;
    }

    return res;
}

GLfloat *gen_cube() {
    return new GLfloat[216] {
         // Positions          // Normals
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
         -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
          0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
          0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
    };
}
