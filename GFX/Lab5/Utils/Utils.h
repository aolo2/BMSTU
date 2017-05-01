#ifndef _UTILS_H
#define _UTILS_H

#define GLEW_STATIC
#include <GL/glew.h>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

enum {
    INVISIBLE = -1,
    BORDER = 0,
    VISIBLE = 1,
};

// translate cursor coordinates to relative (WINDOW_WIDTH, WINDOW_HEIGHT) to (1, 1)
glm::vec2 window_to_relative(double x, double y);

void rebind(GLuint vao, GLuint vbo, const std::vector<glm::vec2> data);

inline int sign(float value) { return (value == 0) ? BORDER : ((value > 0) ? VISIBLE : INVISIBLE); }

glm::vec2 intersection_point(const glm::vec2 &P1, const glm::vec2 &P2, 
                            const glm::vec2 &W1, const glm::vec2 &W2);

int visibility(const glm::vec2 &point, const glm::vec2 &P1, const glm::vec2 &P2);

std::vector<glm::vec2> clip(std::vector<glm::vec2> cutter, std::vector<glm::vec2> cuttie);

#endif