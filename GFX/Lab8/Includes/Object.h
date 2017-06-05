#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include "Shapes.h"
#include "Shader.h"

#include <vector>

namespace Utils {

    class Object {
    public:
        // functional
        void render() const;
        
        // essentials
        Object(Shape shape, Utils::ID materialID);
        virtual ~Object();
        Object(const Object &other);
        
        // setters
                
        // getters
        std::vector<GLfloat> gvertices() const { return vertices; }
        std::vector<GLfloat> gnormals() const { return normals; }
        Utils::ID gmaterialID() const { return materialID; }
    private:
        Object &operator=(Object other);
        void init();
        
        std::vector<GLfloat> vertices;
        std::vector<GLfloat> normals;
        GLuint VAO, VBO;
        Utils::ID materialID;
    };

}