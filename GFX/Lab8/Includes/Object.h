#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Shapes.h"

#include <vector>

namespace Utils {

    class Object {
    public:
        // functional
        void render() const;
        
        // essentials
        Object();
        virtual ~Object();
        Object(const Object &other);
        
        // getters
        GLuint gVAO() const { return VAO; }
        GLuint gVBO() const { return VBO; }
        std::vector<GLfloat> gvertices() const { return vertices; }
        std::vector<GLfloat> gnormals() const { return normals; }
        std::vector<GLfloat> gcolors() const { return colors; }
    private:
        Object &operator=(Object other);
        void init();
        
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> colors;
        std::vector<GLfloat> normals;
        GLuint VAO, VBO;
    };

}