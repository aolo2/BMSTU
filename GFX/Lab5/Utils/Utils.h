#ifndef _UTILS_H
#define _UTILS_H

#define GLEW_STATIC
#include <GL/glew.h>

#define GLM_FORCE_RADIANS

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

namespace Utils {

    enum {
        INVISIBLE = -1,
        BORDER = 0,
        VISIBLE = 1,
    };

    enum Type {
        UNDEFINED = -2,
        ENTRY = -1,
        NONE = 0,
        EXIT = 1,
    };

    struct Vertex {
        glm::vec2 coords;
        float param;
        Type type;
    };

    // translate cursor coordinates to relative (WINDOW_WIDTH, WINDOW_HEIGHT) to (1, 1)
    glm::vec2 window_to_relative(double x, double y);

    void rebind(GLuint VAO, GLuint VBO, const std::vector<glm::vec2> data);

    inline int sign(float value) { return (value == 0) ? BORDER : ((value > 0) ? VISIBLE : INVISIBLE); }

    Vertex intersection_point(const glm::vec2 &P1, const glm::vec2 &P2,
                                    const glm::vec2 &W1, const glm::vec2 &W2);

    int visibility(const glm::vec2 &point, const glm::vec2 &P1, const glm::vec2 &P2);

    std::vector<std::vector<Vertex>> clip(std::vector<glm::vec2> cutter, const std::vector<glm::vec2> &cuttie);
}

#endif
