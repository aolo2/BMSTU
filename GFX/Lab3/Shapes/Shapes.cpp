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
            vertices_vector.push_back(glm::vec3(x, y, z) * 0.35f);
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

GLfloat *gen_surface(unsigned int slices) {
    std::vector<glm::vec2> broken_line;   // (x, y)
    std::vector<GLfloat> surface;

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

    float phi;    // rotating around 'Oy'
    for (int j = 0; j < broken_line.size() - 1; j++) {

        glm::vec2 v = broken_line[j];
        glm::vec2 v_above = broken_line[j + 1];

        for (unsigned int i = 0; i <= slices; i++) {
            phi = 2 * (float) PI / slices * i;
            surface.push_back(v.x * cos(phi));
            surface.push_back(v.y);
            surface.push_back(v.x * sin(phi));

            surface.push_back(v_above.x * cos(phi));
            surface.push_back(v_above.y);
            surface.push_back(v_above.x * sin(phi));
        }
    }

    GLfloat *data = new GLfloat[surface.size()];
    std::copy(surface.begin(), surface.end(), data);

    return data;
}

GLfloat *gen_cube() {
    return new GLfloat[108] {
         // Positions          // Normals
         -0.35f, -0.35f, -0.35f,
          0.35f, -0.35f, -0.35f,
          0.35f,  0.35f, -0.35f,
          0.35f,  0.35f, -0.35f,
         -0.35f,  0.35f, -0.35f,
         -0.35f, -0.35f, -0.35f,

         -0.35f, -0.35f,  0.35f,
          0.35f, -0.35f,  0.35f,
          0.35f,  0.35f,  0.35f,
          0.35f,  0.35f,  0.35f,
         -0.35f,  0.35f,  0.35f,
         -0.35f, -0.35f,  0.35f,

         -0.35f,  0.35f,  0.35f,
         -0.35f,  0.35f, -0.35f,
         -0.35f, -0.35f, -0.35f,
         -0.35f, -0.35f, -0.35f,
         -0.35f, -0.35f,  0.35f,
         -0.35f,  0.35f,  0.35f,

          0.35f,  0.35f,  0.35f,
          0.35f,  0.35f, -0.35f,
          0.35f, -0.35f, -0.35f,
          0.35f, -0.35f, -0.35f,
          0.35f, -0.35f,  0.35f,
          0.35f,  0.35f,  0.35f,

         -0.35f, -0.35f, -0.35f,
          0.35f, -0.35f, -0.35f,
          0.35f, -0.35f,  0.35f,
          0.35f, -0.35f,  0.35f,
         -0.35f, -0.35f,  0.35f,
         -0.35f, -0.35f, -0.35f,

         -0.35f,  0.35f, -0.35f,
          0.35f,  0.35f, -0.35f,
          0.35f,  0.35f,  0.35f,
          0.35f,  0.35f,  0.35f,
         -0.35f,  0.35f,  0.35f,
         -0.35f,  0.35f, -0.35f,
    };
}
