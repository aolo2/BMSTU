#include "Shader/Shader.h"

#include <GLFW/glfw3.h>
#include <vector>

#include <unistd.h>

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

bool fill = false;

struct Edge {
    glm::vec2 start;
    glm::vec2 end;
};

GLuint CANVAS;
std::vector<Edge> edges;
GLubyte *data;

inline int sign(int i) { return i == 0 ? 0 : (i > 0 ? 1 : -1); }
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 4", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    data = (GLubyte *) malloc((WINDOW_WIDTH + 1) * (WINDOW_HEIGHT + 1) * 3 * sizeof(GLubyte));
    memset(data, 0x00, WINDOW_WIDTH * WINDOW_HEIGHT * 3 * sizeof(GLubyte));

    // =======QUAD=============
    GLfloat quad_vertices[] = {
        -1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f,

        1.0f, -1.0f,
        1.0f, 1.0f,
        -1.0f, -1.0f,
    };

    GLuint QUAD_VAO, QUAD_VBO;
    glGenVertexArrays(1, &QUAD_VAO);
    glGenBuffers(1, &QUAD_VBO);

    glBindVertexArray(QUAD_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, QUAD_VBO);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), quad_vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // ========================

    glGenTextures(1, &CANVAS);
    glBindTexture(GL_TEXTURE_2D, CANVAS);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    Shader quad("GLSL/textured_quad.vert", "GLSL/textured_quad.frag");

    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        quad.useProgram();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, CANVAS);

        quad.setUniform1i("canvas", 0);

        glBindVertexArray(QUAD_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    free(data);
    glDeleteBuffers(1, &QUAD_VBO);
    glDeleteTextures(1, &CANVAS);
    glDeleteVertexArrays(1, &QUAD_VAO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void color_pixel_at(int x, int y) {
    x += WINDOW_WIDTH / 2;
    y += WINDOW_HEIGHT / 2;

    data[WINDOW_WIDTH * y * 3 + x * 3 + 0] = 0xFF;
    data[WINDOW_WIDTH * y * 3 + x * 3 + 1] = 0xFF;
    data[WINDOW_WIDTH * y * 3 + x * 3 + 2] = 0xFF;
}

void invert_pixel_at(int x, int y) {
    x += WINDOW_WIDTH / 2;
    y += WINDOW_HEIGHT / 2;

    data[WINDOW_WIDTH * y * 3 + x * 3 + 0] ^= 0xFF;
    data[WINDOW_WIDTH * y * 3 + x * 3 + 1] ^= 0xFF;
    data[WINDOW_WIDTH * y * 3 + x * 3 + 2] ^= 0xFF;
}


bool pixel_at_filled(int x, int y) {
    x += WINDOW_WIDTH / 2;
    y += WINDOW_HEIGHT / 2;

    return data[WINDOW_WIDTH * y * 3 + x * 3] == 0xFF;
}

int intersection_x(const Edge &edge, float y) {
    float k, b;

    if (y > std::max(edge.start.y, edge.end.y) || y < std::min(edge.start.y, edge.end.y)) {
        return -1.0f * WINDOW_WIDTH;
    }

    float dx = edge.end.x - edge.start.x;
    float dy = edge.end.y - edge.start.y;

    // what if dx == 0?
    // vertical line
    if (dx == 0) {
        return edge.end.x;
    }

    k = dy / dx;
    b = edge.end.y - k * edge.end.x;

    float x_int = (y - b) / k;

    // the left-most pixel, center of which is to the right of intersection
    // what if rounded up, but already at the edge of a window?
    int x = std::min((int) round(x_int), WINDOW_WIDTH / 2);
}

void rasterize() {
    memset(data, 0x00, (WINDOW_WIDTH + 1) * (WINDOW_HEIGHT + 1) * 3 * sizeof(GLubyte));

    if (edges.size() >= 2) {
        // connect first and last vertex
        edges[0].start = edges.back().end;

        if (fill) {
            for (float y = WINDOW_HEIGHT / 2 - 0.5f; y >= WINDOW_HEIGHT / -2; y--) {
                for (const Edge &edge : edges) {
                    int x_int = intersection_x(edge, y);
                    if (x_int != -1.0f * WINDOW_WIDTH) {
                        invert_pixel_at(x_int, (int)(y + 0.5f));
                    }
                }
            }

            for (int y = WINDOW_HEIGHT / 2; y >= WINDOW_HEIGHT / -2; y--) {
                bool inside = false;
                for (int x = WINDOW_WIDTH / -2; x <= WINDOW_WIDTH / 2; x++) {
                    if (pixel_at_filled(x, y)) {
                        inside = !inside;
                    }

                    if (inside) {
                        color_pixel_at(x, y);
                    }
                }
            }
        }

        for (Edge edge : edges) {
            int x1 = edge.start.x;
            int y1 = edge.start.y;

            int x2 = edge.end.x;
            int y2 = edge.end.y;

            if (x1 == x2 && y1 == y2) {
                continue;
            }

            int x, y, dx, dy, e, s1, s2;
            x = x1;
            y = y1;

            dx = abs(x2 - x1);
            dy = abs(y2 - y1);

            s1 = sign(x2 - x1);
            s2 = sign(y2 - y1);

            bool swap = false;
            if (dy > dx){
                std::swap(dx, dy);
                swap = true;
            }

            e = 2 * dy - dx;

            for (int j = 1; j <= dx; j++) {
                color_pixel_at(x, y);

                while (e >= 0) {
                    swap ? x += s1 : y += s2;
                    e -= 2 * dx;
                }

                swap ? y += s2 : x += s1;
                e += 2 * dy;
            }
        }
    }

    glBindTexture(GL_TEXTURE_2D, CANVAS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// ===== Callbacks ======
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_SPACE:
                edges.clear();
                rasterize();
                break;
            case GLFW_KEY_ENTER:
                fill = !fill;
                rasterize();
                break;
        }
    }

    if (action == GLFW_RELEASE) { }
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    WINDOW_WIDTH = width, WINDOW_HEIGHT = height;
    glViewport(0, 0, width, height);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);

        Edge edge;
        if (edges.size() > 0) {
            edge.start = edges.back().end;
            edge.end = glm::vec2(x - WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - y);
            edges.push_back(edge);
        } else {
            edge.start = glm::vec2(0, 0);
            edge.end = glm::vec2(x - WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - y);
            edges.push_back(edge);
        }

        rasterize();
    }
}
