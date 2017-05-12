#include "Object.h"

#include <iostream>

Object::Object(Shape shape, GLenum drawmode) : shape(shape), VAO(0), 
               VBO(0), albedo(0), drawmode(drawmode), collision_radius(0.0f) {
                   
    std::vector<glm::vec2> broken_line;
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

    switch (shape) {
        case Shape::SPIN_SURFACE:
            vertices = gen_surface(slices, broken_line);
            broken_points = broken_line.size();
            collision_radius = max_dist(broken_line);
            break;
        case Shape::CUBE:
            collision_radius = sqrt(3) / 2;
            vertices = gen_cube();
            break;
        case Shape::QUAD:
            vertices = gen_quad();
            break;
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    
    switch (this->shape) {
        case Shape::SPIN_SURFACE:
            load_texture("Textures/Materials/albedo.png");     
            
            glBindVertexArray(VAO);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
            glEnableVertexAttribArray(2);
            break;
        case Shape::CUBE:
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            break;
        case Shape::QUAD:
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            break;
    }
    
    glBindVertexArray(0);
}

void Object::render() {
    glBindVertexArray(VAO);
    
    glPolygonMode(GL_FRONT_AND_BACK, drawmode);

    switch (this->shape) {
        case Shape::SPIN_SURFACE:
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, albedo);
            glDrawArrays(GL_TRIANGLE_FAN, 0, (slices + 1));
            glDrawArrays(GL_TRIANGLE_STRIP, (slices + 1), (slices + 1) * (broken_points - 1) * 2);
            glDrawArrays(GL_TRIANGLE_FAN, (slices + 1) * ((broken_points - 1) * 2 + 1), (slices + 1));
            break;
        case Shape::CUBE:
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);
            break;
        case Shape::QUAD:
            glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
            break;
        
    }
    
    glBindVertexArray(0);
}

void Object::load_texture(const std::string &path) {
    
    if (albedo != 0) {
        glDeleteTextures(1, &(this->albedo));
    }
    
    glGenTextures(1, &(this->albedo));
    
    int width, height;
    unsigned char* image = SOIL_load_image(path.c_str(), &width, &height, 0, SOIL_LOAD_RGB); 
    
    glBindTexture(GL_TEXTURE_2D, albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenerateMipmap(GL_TEXTURE_2D);
    
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}

float Object::get_collision_radius() {
    return collision_radius;
}

Object::~Object() {
    glDeleteTextures(1, &albedo);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}