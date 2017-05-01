#include "Utils.h"

#include <iostream>

glm::vec2 window_to_relative(double x, double y) {
    return glm::vec2((x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2), 
                    (WINDOW_HEIGHT / 2 - y) / (WINDOW_HEIGHT / 2));
}

void rebind(GLuint vao, GLuint vbo, const std::vector<glm::vec2> data) {
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, data.size() * 2 * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);
}


std::vector<glm::vec2> clip(std::vector<glm::vec2> cutter, std::vector<glm::vec2> cuttie) {
    cutter.push_back(cutter[0]);
    std::vector<glm::vec2> output(cuttie);
    
    for (int i = 0; i < cutter.size() - 1; i++) {

        glm::vec2 start = cutter[i], end = cutter[i + 1];

        std::vector<glm::vec2> input = output;

        output.clear();
        
        glm::vec2 S = input.back();

        for (const auto &E : input) {
            if (visibility(E, start, end) == VISIBLE) {
                if (visibility(S, start, end) == INVISIBLE) {
                    output.push_back(intersection_point(S, E, start, end));
                }
                output.push_back(E);
            } else if (visibility(S, start, end) == VISIBLE) {
                output.push_back(intersection_point(S, E, start, end));
            }
            S = E;
        }
    }
    
    std::cout << output.size() << std::endl;
    for (auto v :output) {
        printf("(%.3f, %.3f) ", v.x, v.y);
    }
    std::cout << std::endl;
    
    return output;
}


int visibility(const glm::vec2 &point, const glm::vec2 &P1, const glm::vec2 &P2) {
    float tmp1 = (point.x - P1.x) * (P2.y - P1.y);
    float tmp2 = (point.y - P1.y) * (P2.x - P1.x);
    
    return sign(tmp1 - tmp2);
}

glm::vec2 intersection_point(const glm::vec2 &P1, const glm::vec2 &P2, 
                                const glm::vec2 &W1, const glm::vec2 &W2) {
    glm::mat2 coeff;
    coeff[0][0] = P2.x - P1.x;
    coeff[1][0] = W1.x - W2.x;
    coeff[0][1] = P2.y - P1.y;
    coeff[1][1] = W1.y - W2.y;
    
    glm::vec2 right(W1.x - P1.x, W1.y - P1.y);
    glm::vec2 param = glm::inverse(coeff) * right;
    
    return P1 + (P2 - P1) * param.x;
}