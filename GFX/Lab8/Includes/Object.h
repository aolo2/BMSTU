#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Shapes.h"

#include <vector>

namespace Utils {

    class Object {
    public:
        // functional
        void render();
        
        // essentials
        Object();
        virtual ~Object();
        Object(const Object &other);
        Object &operator=(Object other);
        void swap(Object &other);
        
        // getters
        GLuint gVAO() const { return VAO; }
        GLuint gVBO() const { return VBO; }
        std::vector<GLfloat> gvertices() const { return vertices; }
    private:
        std::vector<GLfloat> vertices;
        GLuint VAO, VBO;
    };

}