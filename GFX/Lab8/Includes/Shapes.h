#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

namespace Shapes {

    std::vector<GLfloat> cube();
    std::vector<GLfloat> sphere();
    
}