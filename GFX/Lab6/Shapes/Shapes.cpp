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
    std::vector<glm::vec2> broken_line;   // in (x, y, 0)
    std::vector<GLfloat> surface;
    std::vector<GLfloat> full_surface;

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
        
        glm::vec2 dir(v_above - v);
        glm::vec2 norm(dir.y, -1 * dir.x);

        for (unsigned int i = 0; i <= slices; i++) {
            phi = 2 * (float) PI / slices * i;
            
            // lab7 : remember cos, sin?
            
            glm::vec3 this_point(v.x * cos(phi), v.y, v.x * sin(phi));
            glm::vec3 next_point(v_above.x * cos(phi), v_above.y, v_above.x * sin(phi));
            glm::vec3 norm3(norm.x * cos(phi), norm.y, norm.x * sin(phi));
            // glm::vec3 norm3 = glm::mat3(rot) * glm::vec3(norm, 0.0f);
            
            surface.push_back(this_point.x);
            surface.push_back(this_point.y);
            surface.push_back(this_point.z);
            
            surface.push_back(norm3.x);
            surface.push_back(norm3.y);
            surface.push_back(norm3.z);
            
            surface.push_back(next_point.x);
            surface.push_back(next_point.y);
            surface.push_back(next_point.z);
            
            surface.push_back(norm3.x);
            surface.push_back(norm3.y);
            surface.push_back(norm3.z);
        }
    }

    for (int i = 0; i <= slices; i++) {
        full_surface.push_back(surface[i * 12 + 0]);
        full_surface.push_back(surface[i * 12 + 1]);
        full_surface.push_back(surface[i * 12 + 2]);
        
        full_surface.push_back(0.0f);
        full_surface.push_back(-1.0f);
        full_surface.push_back(0.0f);
    }
    
    full_surface.insert(full_surface.end(), surface.begin(), surface.end());
    
    for (int i = 0; i <= slices; i++) {
        full_surface.push_back(surface[surface.size() - 6 - i * 12]);
        full_surface.push_back(surface[surface.size() - 5 - i * 12]);
        full_surface.push_back(surface[surface.size() - 4 - i * 12]);
        
        full_surface.push_back(0.0f);
        full_surface.push_back(1.0f);
        full_surface.push_back(0.0f);
    }

    
    // std::cout << full_surface.size() << std::endl;
    // 2772 (slices = 20)
    
    GLfloat *data = new GLfloat[full_surface.size()];
    std::copy(full_surface.begin(), full_surface.end(), data);
        
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
