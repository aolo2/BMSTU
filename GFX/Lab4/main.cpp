#include "Shader/Shader.h"
#include "Utility/Utils.h"

#include <GLFW/glfw3.h>

GLuint CANVAS;
std::vector<Edge> edges;
bool fill, antialias, determined, righthand;

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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    data = (GLubyte *) malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 3 * sizeof(GLubyte));
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

void rasterize() {
    memset(data, 0x00, WINDOW_WIDTH * WINDOW_HEIGHT * 3 * sizeof(GLubyte));

    if (edges.size() < 3) {
        glBindTexture(GL_TEXTURE_2D, CANVAS);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        return;
    }
     
    // connect first and last vertex
    edges[0].start = edges.back().end;

    if (fill) {
        // find pairs pixels on each scanning line (step 1)
        for (float y = WINDOW_HEIGHT / 2 + 0.5f; y > WINDOW_HEIGHT / -2; y--) {
            for (const Edge &relative_edge : edges) {
                Edge edge = recalculate(relative_edge);
                
                int x_int = intersection_x(edge, y);
                if (x_int != -1.0f * WINDOW_WIDTH) {
                    invert_pixel_at(x_int, y - 0.5f);
                }
            }
        }
        
        // fill everything inbetween pairs of pixels (step 2)
        for (int y = WINDOW_HEIGHT / 2; y > WINDOW_HEIGHT / -2; y--) {
            bool inside = false;
            for (int x = WINDOW_WIDTH / -2; x < WINDOW_WIDTH / 2; x++) {
                if (pixel_at_filled(x, y)) {
                    inside = !inside;
                }
                    if (inside) {
                    color_pixel_at(x, y, 8.0f);
                }
            }
        }
    }
    
    
    if (antialias) {
        int I = 8;
        glm::mat2 rot;
        
        if (!determined) {
            Edge e1 = edges[0];
            float dx = (float) (e1.end.x - e1.start.x);
            float dy = (float) (e1.end.y - e1.start.y);
            
            float angle = - atan2(dy, dx) + M_PI / 2;
            
            rot[0][0] = cos(angle);
            rot[1][0] = -sin(angle);
            rot[0][1] = sin(angle);
            rot[1][1] = cos(angle);
            
            glm::vec2 e2 = edges[1].end;
            
            float edge_end_x = (rot * e1.end).x;
            float another_x = (rot * e2).x;
            
            righthand = (another_x > edge_end_x);
            determined = true;
        }
        
        for (const Edge &relative_edge : edges) {
            Edge edge = recalculate(relative_edge);
            
            int x1 = edge.start.x;
            int y1 = edge.start.y;

            int x2 = edge.end.x;
            int y2 = edge.end.y;

            int octant = octant_number(x2 - x1, y2 - y1);
            
            int x, y, dx, dy, s1, s2;
            float e;
            
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
            
            float m = ((float) I * dy) / dx;
            float w = I - m;
            e = I / 2.0f;
            
            color_pixel_at(x, y, m / 2);
            for (int i = 1; i <= dx; i++) {
                if (e < w) {
                    swap ? y += s2 : x += s1;
                    e += m;
                } else {
                    if (swap) {
                        y += s2;
                        x += s1;
                    } else {
                        y += s2;
                        x += s1;
                    }
                    e -= w;
                    
                }
                
                if (righthand) {
                    color_pixel_at(x, y, (octant % 2 != 0) ? e : 8.0f - e);
                } else {
                    color_pixel_at(x, y, (octant % 2 != 0) ? 8.0f - e : e);
                }
            }
        }
        
    } else { // if !antialias
        for (const Edge &relative_edge : edges) {
            Edge edge = recalculate(relative_edge);
            
            int x1 = edge.start.x;
            int y1 = edge.start.y;

            int x2 = edge.end.x;
            int y2 = edge.end.y;

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
                color_pixel_at(x, y, 8.0f);

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
            case GLFW_KEY_TAB:
                antialias = !antialias;
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
    data = (GLubyte *) realloc(data, width * height * 3 * sizeof(GLubyte));
    WINDOW_WIDTH = width, WINDOW_HEIGHT = height;
    rasterize();
    glViewport(0, 0, width, height);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        // (0, 0) to (WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
        // std::cout << x << " " << y << std::endl;
        glm::vec2 cursor_pos = px_to_screen(x, y);
        
        Edge edge;
        if (edges.size() > 0) {
            edge.start = edges.back().end;
            edge.end = cursor_pos;
            if (edge.start != edge.end) {
                edges.push_back(edge);
                determined = false;
                rasterize();
            }
        } else {
            edge.start = glm::vec2(0, 0);
            edge.end = cursor_pos;
            if (edge.start != edge.end) {
                edges.push_back(edge);
                determined = false;
                rasterize();
            }
        }
    }
}