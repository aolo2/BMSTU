#include "Shader.h"

#include <GLFW/glfw3.h>
#include <vector>
#include <set>
#include <map>

typedef std::pair<unsigned int, unsigned int> patch;
typedef std::pair<patch, patch> patch_pair;

const int WINDOW_WIDTH = 80;
const int WINDOW_HEIGHT = 45;

const float TRESHHOLD = 0.01f;
const int LOD = 3;

GLuint CANVAS;
GLubyte *data;

bool mouse_pressed = false, changes_to_data = false;
unsigned int x_pos = WINDOW_WIDTH / 2, y_pos = WINDOW_HEIGHT / 2;

std::set<patch> patch_set;
std::vector<std::vector<glm::vec2>> polygons;

std::map<patch_pair, float> factors;
std::map<patch, float> energy;
std::map<patch, float> unshot_energy;
std::set<patch> emitters;

// pair of coordinates to vector
glm::vec2 light_pos = {WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};

inline int sign(int i) { return i == 0 ? 0 : (i > 0 ? 1 : -1); }

void update();

unsigned int dist(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    return (unsigned int) std::round(glm::sqrt((x2 - x1) * (x2 - x1) +
                                               (y2 - y1) * ((y2 - y1))));
}

void set_pixel(unsigned int x, unsigned int y, GLubyte value) {
    data[y * WINDOW_WIDTH * 3 * sizeof(GLubyte) + x * 3 * sizeof(GLubyte) + 0] = value;
    data[y * WINDOW_WIDTH * 3 * sizeof(GLubyte) + x * 3 * sizeof(GLubyte) + 1] = value;
    data[y * WINDOW_WIDTH * 3 * sizeof(GLubyte) + x * 3 * sizeof(GLubyte) + 2] = value;
}

void my_swap(int &a, int &b) {
    a ^= b;
    b ^= a;
    a ^= b;
}

void clear_all() {
    memset(data, 0x00, WINDOW_WIDTH * WINDOW_HEIGHT * 3 * sizeof(GLubyte));
    patch_set.clear();

    polygons.clear();
    polygons.emplace_back();
}

void clear_canvas() {
    memset(data, 0x00, WINDOW_WIDTH * WINDOW_HEIGHT * 3 * sizeof(GLubyte));
}

bool intersection(const glm::vec2 &P1, const glm::vec2 &P2,
                  const glm::vec2 &W1, const glm::vec2 &W2) {
    float s1x, s1y, e1x, e1y, s2x, s2y, e2x, e2y;
    s1x = P1.x, s1y = P1.y, e1x = P2.x, e1y = P2.y;
    s2x = W1.x, s2y = W1.y, e2x = W2.x, e2y = W2.y;

    float ax = s1x, ay = s1y;
    float bx = e1x - s1x, by = e1y - s1y;

    float cx = s2x, cy = s2y;
    float dx = e2x - s2x, dy = e2y - s2y;

    float u = (bx * (cy - ay) + by * (ax - cx)) / (dx * by - dy * bx);
    float t = (dx * (ay - cy) + dy * (cx - ax)) / (bx * dy - by * dx);

    return ((u >= 0.0f) && (u <= 1.0f) && (t >= 0.0f) && (t <= 1.0f));
}

float single_factor(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    bool intersects = false;
    for (const auto &polygon : polygons) {
        for (int i = 0; i < polygon.size(); i++) {
            glm::vec2 from = polygon[i];
            glm::vec2 to = (i == polygon.size() - 1) ? polygon[0] : polygon[i + 1];

            if (intersection({x2, y2}, {x1, y1}, from, to)) {
                intersects = true;
                break;
            }
        }
    }

    return intersects ? 0.0f : (0.2f / (dist(x1, y1, x2, y2)));
}

//void form_factors() {
//
//    for (unsigned int x1 = 0; x1 < WINDOW_WIDTH; x1 += LOD) {
//        for (unsigned int y1 = 0; y1 < WINDOW_HEIGHT; y1 += LOD) {
//
//            if (patch_set.find({x1, y1}) != patch_set.end()) { continue; }
//
//            for (unsigned int x2 = 0; x2 < WINDOW_WIDTH; x2 += LOD) {
//                for (unsigned int y2 = 0; y2 < WINDOW_HEIGHT; y2 += LOD) {
//
//
//                    if (patch_set.find({x2, y2}) != patch_set.end() || (x1 == x2 && y1 == y2)) { continue; }
//
//                    bool intersects = false;
//                    for (int i = 0; i < polygon.size(); i++) {
//                        glm::vec2 from = polygon[i];
//                        glm::vec2 to = (i == polygon.size() - 1) ? polygon[0] : polygon[i + 1];
//                        // only the last one works... ?
//
//                        if (intersection({x2, y2}, {x1, y1}, from, to)) {
//                            intersects = true;
//                            break;
//                        }
//                    }
//
//                    if (!intersects) {
//                        factors[{{x2, y2},
//                                 {x1, y1}}] = factors[{{x1, y1},
//                                                       {x2, y2}}] =
//                                0.2f / (dist(x1, y1, x2, y2));
//                    } else {
//                        factors[{{x2, y2},
//                                 {x1, y1}}] = factors[{{x1, y1},
//                                                       {x2, y2}}] = 0.0f;
//                    }
//                }
//            }
//        }
//    }
//}

void radiate() {

    std::map<patch, float> next_unshot;

    for (unsigned int x = 0; x < WINDOW_WIDTH; x += LOD) {
        std::cout << (float) x / WINDOW_WIDTH << std::endl;
        for (unsigned int y = 0; y < WINDOW_HEIGHT; y += LOD) {

//            if (patch_set.find({x, y}) != patch_set.end()) { continue; }

            for (const auto &e : unshot_energy) {
                if (e.first != std::make_pair(x, y)) {

                    float ue, f;
                    ue = e.second;
                    f = single_factor(x, y, e.first.first, e.first.second);

                    if (ue * f >= TRESHHOLD) {
                        next_unshot[{x, y}] = ue * f;
                    } else continue;

                    next_unshot[{x, y}] *= glm::pow(LOD, 2);

                    energy[{x, y}] = std::min(ue * f + energy[{x, y}], 255.0f);
                }
            }
        }
    }

//    std::cout << next_unshot.size() << std::endl;
    unshot_energy = next_unshot;
}

void bresenham(const std::vector<glm::vec2> &polygon) {
    if (polygon.size() < 3) {
        return;
    }

    clear_canvas();

    for (unsigned int i = 0; i < polygon.size(); i++) {
        glm::vec2 from = polygon[i];
        glm::vec2 to = (i == polygon.size() - 1) ? polygon[0] : polygon[i + 1];

        auto x1 = (int) from.x;
        auto y1 = (int) from.y;

        auto x2 = (int) to.x;
        auto y2 = (int) to.y;

        glm::vec2 normal = {x2 - x1, y1 - y2};

        int x, y, dx, dy, e;
        int s1, s2;

        x = x1;
        y = y1;

        dx = glm::abs(x2 - x1);
        dy = glm::abs(y2 - y1);

        s1 = sign(x2 - x1);
        s2 = sign(y2 - y1);

        bool swap = false;
        if (dy > dx) {
            my_swap(dx, dy);
            swap = true;
        }

        e = 2 * dy - dx;

        for (int j = 1; j <= dx; j++) {
//            set_pixel((unsigned int) x, (unsigned int) y, 0xAA);
            patch_set.emplace((unsigned int) x, (unsigned int) y);

            while (e >= 0) {
                swap ? x += s1 : y += s2;
                e -= 2 * dx;
            }

            swap ? y += s2 : x += s1;
            e += 2 * dy;
        }
    }
}

void shade() {
    for (unsigned int x = 0; x < WINDOW_WIDTH; x++) {
        for (unsigned int y = 0; y < WINDOW_HEIGHT; y++) {
            if (energy.find({x, y}) != energy.end()) {
                set_pixel(x, y, static_cast<GLubyte>(std::min(energy[{x, y}], 255.0f)));
//                std::cout << energy << " ";
            } else {
                set_pixel(x, y, 0x00);
            }
        }
//        std::cout << std::endl;
    }
}

void update() {

    for (const auto &p : polygons) {
        bresenham(p);
    }

    shade();

//    set_pixel(40, 20, 0xFF);
//    set_pixel(70, 20, 0xFF);

    glBindTexture(GL_TEXTURE_2D, CANVAS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void render(GLuint QUAD_VAO) {
    changes_to_data = false;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, CANVAS);

    glBindVertexArray(QUAD_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        double t1;

        switch (key) {
            case GLFW_KEY_N:
                polygons.emplace_back();
                break;
            case GLFW_KEY_F:
                t1 = glfwGetTime();
//                form_factors();
                // TODO: multi-thread (mb compute shaders??)
                std::cout << glfwGetTime() - t1 << std::endl;
                break;
            case GLFW_KEY_R:
                t1 = glfwGetTime();
                radiate();
                std::cout << glfwGetTime() - t1 << std::endl;
                break;
            case GLFW_KEY_U:
                update();
                break;
            case GLFW_KEY_SPACE:
                clear_all();
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            default:
                std::cout << key << std::endl;
                break;
        }
    }
}

void add_vertex() {
    if (patch_set.find({x_pos, y_pos}) == patch_set.end()
        && x_pos < WINDOW_WIDTH && y_pos < WINDOW_HEIGHT) {
        patch_set.emplace(x_pos, y_pos);
        polygons.back().emplace_back(x_pos, y_pos);
        changes_to_data = true;
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mouse_pressed = true;
            add_vertex();
        } else {
            mouse_pressed = false;
        }
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    x_pos = (unsigned int) std::round(xpos / 16 - 0.5f);
    y_pos = WINDOW_HEIGHT - (unsigned int) std::round(ypos / 16 + 0.5f);

    if (mouse_pressed) {
        add_vertex();
    }
}

/* WORKFLOW:
 *
 * 1. Add initial geometry
 * 2. Add initial emitters
 * 3. Press a button - radiosity passed start
 * 4. Press a button - -//- stop
 *
 *
 * NEED TO FIGURE OUT:
 *
 * 1. How to do 2D form-factors? (what to do with corners/how to know if that is a corner?)
 * 1.5. How to do blockers (ray-casting??? pls no)
 * 2. Speed up (to slow even for 100 patches)
 * 3. How to do colors? (lighting is added!!)


 * 4. Real-time WTF
 * */

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH * 16, WINDOW_HEIGHT * 16, "...", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwSetCursorPos(window, WINDOW_WIDTH * 16 / 2, WINDOW_HEIGHT * 16 / 2);

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

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *)
            nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // ========================


    // =======Texture==========
    glGenTextures(1, &CANVAS);
    glBindTexture(GL_TEXTURE_2D, CANVAS);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    // ========================

    Utils::Shader quad("GLSL/texture.vert", "GLSL/texture.frag");

    quad.useProgram();
    quad.setUniform1i("canvas", 0);

    glViewport(0, 0, WINDOW_WIDTH * 16, WINDOW_HEIGHT * 16);

    polygons.emplace_back();

    unshot_energy[{light_pos.x, light_pos.y}] = 255.0f;

    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        render(QUAD_VAO);

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
