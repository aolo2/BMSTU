#include "Shapes.h"
#include <iostream>

float max_dist(const std::vector<glm::vec2> &line) {
    float max_dist = 0.0f;
    
    for (const auto &point : line) {
        float dist = sqrt(point.x * point.x + point.y * point.y);
        if (dist > max_dist) { 
            max_dist = dist;
        }
    }
    
    return max_dist;
}

std::vector<GLfloat> gen_quad() {
    std::vector<GLfloat> v = 
    {
        -1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f,

        -1.0f, -1.0f,
         1.0f,  1.0f,
         1.0f, -1.0f,
    };
    
    return v;
}

std::vector<GLfloat> gen_surface(unsigned int slices, const std::vector<glm::vec2> &broken_line) {
    std::vector<GLfloat> surface;
    std::vector<GLfloat> full_surface;

    for (int j = 0; j < broken_line.size() - 1; j++) {

        glm::vec2 v = broken_line[j];
        glm::vec2 v_above = broken_line[j + 1];
        glm::vec2 dir(v_above - v);
        glm::vec2 norm(dir.y, -1 * dir.x);
        
        float phi;
        
        for (unsigned int i = 0; i <= slices; i++) {
            phi = 2 * (float) PI / slices * i;
            
            // lab7 : remember cos, sin?
            
            glm::vec3 this_point(v.x * cos(phi), v.y, v.x * sin(phi));
            glm::vec3 next_point(v_above.x * cos(phi), v_above.y, v_above.x * sin(phi));
            glm::vec3 norm3(norm.x * cos(phi), norm.y, norm.x * sin(phi));
            
            surface.push_back(this_point.x);
            surface.push_back(this_point.y);
            surface.push_back(this_point.z);
            
            surface.push_back(norm3.x);
            surface.push_back(norm3.y);
            surface.push_back(norm3.z);
            
            surface.push_back(i / static_cast<float>(slices));
            surface.push_back(j / static_cast<float>(broken_line.size() - 1));
            
            surface.push_back(next_point.x);
            surface.push_back(next_point.y);
            surface.push_back(next_point.z);
            
            surface.push_back(norm3.x);
            surface.push_back(norm3.y);
            surface.push_back(norm3.z);
            
            surface.push_back(i / static_cast<float>(slices) );
            surface.push_back((j + 1) / static_cast<float>(broken_line.size()));
        }
    }

    for (int i = 0; i <= slices; i++) {
        full_surface.push_back(surface[i * 16 + 0]);
        full_surface.push_back(surface[i * 16 + 1]);
        full_surface.push_back(surface[i * 16 + 2]);
       
        full_surface.push_back(0.0f);
        full_surface.push_back(-1.0f);
        full_surface.push_back(0.0f);
        
        full_surface.push_back(surface[i * 16 + 0] / (broken_line[0].x * 8.0f)); // [-1, 1] -> [0, 1]
        full_surface.push_back(surface[i * 16 + 2] / (broken_line[0].x * 8.0f)); // [-1, 1] -> [0, 1]
    }
    
    full_surface.insert(full_surface.end(), surface.begin(), surface.end());
    
    for (int i = 0; i <= slices; i++) {
        full_surface.push_back(surface[surface.size() - 8 - i * 16]);
        full_surface.push_back(surface[surface.size() - 7 - i * 16]);
        full_surface.push_back(surface[surface.size() - 6 - i * 16]);
        
        full_surface.push_back(0.0f);
        full_surface.push_back(1.0f);
        full_surface.push_back(0.0f);
        
        full_surface.push_back(surface[surface.size() - 8 - i * 16] / (broken_line.back().x * 8.0f)); // [-1, 1] -> [0, 1]
        full_surface.push_back(surface[surface.size() - 6 - i * 16] / (broken_line.back().x * 8.0f));
    }

    return full_surface;
}

std::vector<GLfloat> gen_cube() {
    std::vector<GLfloat> v = 
    {
         -2.0f, -2.0f, -2.0f,
          2.0f, -2.0f, -2.0f,
          2.0f,  2.0f, -2.0f,
          2.0f,  2.0f, -2.0f,
         -2.0f,  2.0f, -2.0f,
         -2.0f, -2.0f, -2.0f,

         -2.0f, -2.0f,  2.0f,
          2.0f, -2.0f,  2.0f,
          2.0f,  2.0f,  2.0f,
          2.0f,  2.0f,  2.0f,
         -2.0f,  2.0f,  2.0f,
         -2.0f, -2.0f,  2.0f,

         -2.0f,  2.0f,  2.0f,
         -2.0f,  2.0f, -2.0f,
         -2.0f, -2.0f, -2.0f,
         -2.0f, -2.0f, -2.0f,
         -2.0f, -2.0f,  2.0f,
         -2.0f,  2.0f,  2.0f,

          2.0f,  2.0f,  2.0f,
          2.0f,  2.0f, -2.0f,
          2.0f, -2.0f, -2.0f,
          2.0f, -2.0f, -2.0f,
          2.0f, -2.0f,  2.0f,
          2.0f,  2.0f,  2.0f,

         -2.0f, -2.0f, -2.0f,
          2.0f, -2.0f, -2.0f,
          2.0f, -2.0f,  2.0f,
          2.0f, -2.0f,  2.0f,
         -2.0f, -2.0f,  2.0f,
         -2.0f, -2.0f, -2.0f,

         -2.0f,  2.0f, -2.0f,
          2.0f,  2.0f, -2.0f,
          2.0f,  2.0f,  2.0f,
          2.0f,  2.0f,  2.0f,
         -2.0f,  2.0f,  2.0f,
         -2.0f,  2.0f, -2.0f,
    };
    
    return v;
}
