#include "Utils.h"

#include <iostream>
#include <algorithm>

namespace Utils {

    bool cmp(const Vertex &a, const Vertex &b) {
        return a.param < b.param;
    }

    glm::vec2 window_to_relative(double x, double y) {
        return glm::vec2((x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2),
                        (WINDOW_HEIGHT / 2 - y) / (WINDOW_HEIGHT / 2));
    }

    void rebind(GLuint VAO, GLuint VBO, const std::vector<glm::vec2> data) {
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, data.size() * 2 * sizeof(GLfloat), data.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    std::vector<std::vector<Vertex>> clip(std::vector<glm::vec2> cutter, const std::vector<glm::vec2> &cuttie) {
        std::vector<std::vector<Vertex>> output, input;

        std::vector<Vertex> tmp;
        for (const auto &v : cuttie) {
            tmp.push_back({ v });
        }

        output.push_back(tmp);

        cutter.push_back(cutter.front());

        for (unsigned int i = 0; i < cutter.size() - 1; i++) {
            glm::vec2 W1(cutter[i]), W2(cutter[i + 1]); // cutter edge

            input = output;
            output.clear();

            // for each (possibly 1) polygon
            for (const auto &vec : input) {

                // clipped points of this polygon
                std::vector<Vertex> this_output;
                Vertex S = vec.back();

                for (const auto &E : vec) {
                    if (visibility(E.coords, W1, W2) == VISIBLE) {
                        if (visibility(S.coords, W1, W2) == INVISIBLE) {
                            this_output.push_back(intersection_point(S.coords, E.coords, W1, W2));
                        }
                        this_output.push_back({ E.coords, 0, Type::NONE });
                    } else if (visibility(S.coords, W1, W2) == VISIBLE) {
                        this_output.push_back(intersection_point(S.coords, E.coords, W1, W2));
                    }

                    S = E;
                }

                // indices of intersection points
                std::vector<unsigned int> indices;
                for (unsigned int i = 0; i < this_output.size(); i++) {
                    if (this_output[i].type == Type::UNDEFINED) { indices.push_back(i); }
                }

                // if there were no intersections, pass only the points
                if (indices.size() == 0) {
                    output.push_back(this_output);
                    continue;
                }



                // determine intersection point's type (exit or entry)
                std::sort(indices.begin(), indices.end(),
                [&](unsigned int a, unsigned int b) { return this_output[a].param < this_output[b].param; });

                for (unsigned int i = 0; i < indices.size(); i++) {
                    this_output[indices[i]].type = ((i % 2) == 0) ? Type::ENTRY : Type::EXIT;
                }

                bool entered = false, false_edges = false;
                for (const auto &v : this_output) {
                    if (v.type == Type::EXIT) {
                        entered = true;
                    } else if (v.type == Type::ENTRY) {
                        if (entered) {
                            false_edges = true;
                        }
                        entered = false;
                    } else {
                        entered = false;
                    }
                }

                if (!false_edges) {
                    output.push_back(this_output);
                    continue;
                }

                // split into new polygons
                std::vector<Vertex> buffer;
                unsigned int added = 0;
                for (unsigned int i = 0; i < this_output.size(); i++) {
                    std::cout << added << " out of " << this_output.size() << std::endl;
                    if (added >= this_output.size()) { break; }

                    if (this_output[i].type == Type::ENTRY) {
                        buffer.clear();
                        buffer.push_back(this_output[i]);
                    } else if (this_output[i].type == Type::EXIT) {
                        if (buffer.size() > 0) {
                            buffer.push_back(this_output[i]);
                            output.push_back(buffer);
                            added += buffer.size();
                            buffer.clear();
                        }
                    } else if (buffer.size() > 0) {
                        buffer.push_back(this_output[i]);
                    }

                    if (i == this_output.size() - 1) {
                        i = 0;
                    }
                }
            }
        }


        std::cout << output.size() << std::endl;
        for (const auto &vec : output) {
            std::cout << vec.size() << ": ";
            for (const auto &v : vec) {
                std::cout << v.coords.x << " " << v.coords.y << std::endl;
            }
            std::cout << std::endl;
        }

        return output;
    }

    int visibility(const glm::vec2 &point, const glm::vec2 &P1, const glm::vec2 &P2) {
        float tmp1 = (point.x - P1.x) * (P2.y - P1.y);
        float tmp2 = (point.y - P1.y) * (P2.x - P1.x);

        return sign(tmp1 - tmp2);
    }

    Vertex intersection_point(const glm::vec2 &P1, const glm::vec2 &P2,
                                    const glm::vec2 &W1, const glm::vec2 &W2) {
        Vertex ret;

        glm::vec2 a(P2 - P1), b(W2 - W1), c(W1 - P1);
        glm::vec2 a_t(-1.0f * a.y, a.x), b_t(-1.0f * b.y, b.x);

        ret.param = glm::dot(a_t, c) / glm::dot(b_t, a);
        ret.coords = W1 + b * ret.param;
        ret.type = Type::UNDEFINED;

        return ret;
    }
}


/*

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

return output;

*/
