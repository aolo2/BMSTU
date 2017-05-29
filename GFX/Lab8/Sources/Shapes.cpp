#include "../Includes/Shapes.h"

namespace Utils::Colors {

std::vector<GLfloat> cube() {
    return std::vector<GLfloat> {
        0.0f,  0.5f,  0.0f,
        0.0f,  0.5f,  0.0f,
        0.0f,  0.5f,  0.0f,
        0.0f,  0.5f,  0.0f,
        0.0f,  0.5f,  0.0f,
        0.0f,  0.5f,  0.0f,

        0.5f,  0.0f,  0.0f,
        0.5f,  0.0f,  0.0f,
        0.5f,  0.0f,  0.0f,
        0.5f,  0.0f,  0.0f,
        0.5f,  0.0f,  0.0f,
        0.5f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
   };
}

}

namespace Utils::Normals {

std::vector<GLfloat> cube() {
    return std::vector<GLfloat> {
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,
        0.0f,  0.0f,  1.0f,

        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,
        0.0f,  0.0f, -1.0f,

        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,
        1.0f,  0.0f,  0.0f,

       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,
       -1.0f,  0.0f,  0.0f,

        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,
        0.0f,  1.0f,  0.0f,

        0.0f,  -1.0f,  0.0f,
        0.0f,  -1.0f,  0.0f,
        0.0f,  -1.0f,  0.0f,
        0.0f,  -1.0f,  0.0f,
        0.0f,  -1.0f,  0.0f,
        0.0f,  -1.0f,  0.0f,
   };
}

}

namespace Utils::Shapes {

std::vector<GLfloat> cube() {
    return std::vector<GLfloat> {
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,

        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,

       -0.5f,  0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,
       -0.5f,  0.5f,  0.5f,

        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,

        0.5f, -0.5f, -0.5f,
       -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
       -0.5f, -0.5f, -0.5f,

       -0.5f,  0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        0.5f,  0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f,  0.5f,
       -0.5f,  0.5f, -0.5f,
   };
}

std::vector<GLfloat> sphere(unsigned int slices) {
    std::vector<glm::vec3> vertices_vector;
    std::vector<int> index_vector;

    for (unsigned int i = 0; i <= slices; i++) {
        float V = static_cast<float>(i) / slices;
        float phi = V * M_PI;

        for (unsigned int j = 0; j <= slices; j++) {
            float U = static_cast<float>(j) / slices;
            float theta = U * 2 * M_PI;

            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            vertices_vector.push_back(glm::vec3(x, y, z) * 0.35f);
        }
    }


    for (int i = 0; i < slices * slices + slices; i++){
        index_vector.push_back(i);
        index_vector.push_back(i + slices + 1);
        index_vector.push_back(i + slices);

        index_vector.push_back(i + slices + 1);
        index_vector.push_back(i);
        index_vector.push_back(i + 1);
    }

    std::vector<float> res;
    for (int i = 0, j = 0; i < index_vector.size(); i++, j+= 3) {

        glm::vec3 tmp = vertices_vector[index_vector[i]];

        res.push_back(tmp.x);
        res.push_back(tmp.y);
        res.push_back(tmp.z);
    }

    return res;
}

}