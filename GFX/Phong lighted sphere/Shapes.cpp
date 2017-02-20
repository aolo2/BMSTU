#include "Shapes.h"

void gen_sphere(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector, 
                float radius, int slices, int stacks) {
                    
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
            vertices_vector.push_back(glm::vec3(x, y, z) * radius);
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
}

void gen_cube(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector, float half_side) {
    vertices_vector.push_back(glm::vec3(-1 * half_side, half_side, -1 * half_side)); // 0
    vertices_vector.push_back(glm::vec3(half_side, half_side, -1 * half_side)); // 1
    vertices_vector.push_back(glm::vec3(-1 * half_side, half_side, half_side)); // 2
    vertices_vector.push_back(glm::vec3(half_side, half_side, half_side)); // 3
    
    vertices_vector.push_back(glm::vec3(-1 * half_side, -1 * half_side, -1 * half_side)); // 4
    vertices_vector.push_back(glm::vec3(half_side, -1 * half_side, -1 * half_side)); // 5
    vertices_vector.push_back(glm::vec3(-1 * half_side, -1 * half_side, half_side)); // 6
    vertices_vector.push_back(glm::vec3(half_side, -1 * half_side, half_side)); // 7
    
    /*
    
    0----1
    |    |
    2----3
    
    
    4----5
    |    |
    6----7
    
    */
    
    // top face
    index_vector.push_back(0);
    index_vector.push_back(1);
    index_vector.push_back(2);
    index_vector.push_back(1);
    index_vector.push_back(2);
    index_vector.push_back(3);
    
    
    // bottom face
    index_vector.push_back(4);
    index_vector.push_back(5);
    index_vector.push_back(6);
    index_vector.push_back(5);
    index_vector.push_back(6);
    index_vector.push_back(7);
    
    
    // front face
    index_vector.push_back(2);
    index_vector.push_back(3);
    index_vector.push_back(6);
    index_vector.push_back(3);
    index_vector.push_back(6);
    index_vector.push_back(7);
    
    
    // right face
    index_vector.push_back(1);
    index_vector.push_back(3);
    index_vector.push_back(7);
    index_vector.push_back(1);
    index_vector.push_back(5);
    index_vector.push_back(7);
    
    
    // back face
    index_vector.push_back(0);
    index_vector.push_back(1);
    index_vector.push_back(4);
    index_vector.push_back(1);
    index_vector.push_back(4);
    index_vector.push_back(5);
    
    
    // left face
    index_vector.push_back(0);
    index_vector.push_back(2);
    index_vector.push_back(4);
    index_vector.push_back(2);
    index_vector.push_back(4);
    index_vector.push_back(6);
}