#include "Shader/Shader.h"
#include "Shapes/Shapes.h"
#include "Trans/Trans.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

#include <queue>

#define MAX_QUEUE 5

int scroll, slices = 20;
float absscale = 1.0f;
bool wireframe, lmb;

GLfloat *surface;
GLuint VAO[2], VBO[2];
glm::mat4 ortho = glm::ortho((float) WINDOW_WIDTH / (float) WINDOW_HEIGHT * -1.0f,
    (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f);
std::queue<glm::vec2> cursor_positions;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

glm::mat4 horizontalIsometry();
glm::vec2 pixel_to_window(glm::vec2 src);
void clear(std::queue<glm::vec2> &q);
void cursor_vec(double xpos, double ypos);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 3", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    surface = gen_surface(slices);
    GLfloat *cube = gen_cube();

    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), cube, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, (slices + 1) * 10 * 6 * sizeof(GLfloat), surface, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");

    glm::mat4 demo_model, view, model, proj;
    glm::mat3 inv_model;

    glm::mat4 customProjMatrix = horizontalIsometry();

    glm::vec3 demo_pos(-0.4f, -0.4f, 0.5f), position, rotate_vec(0.0f, 1.0f, 0.0f);
    float demo_scale = 0.2f, scale, rotate_angle = 1.0f;
    double cursor_x, cursor_y;

    demo_model = glm::translate(demo_model, demo_pos);
    demo_model = glm::scale(demo_model, glm::vec3(demo_scale));

    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        proj = customProjMatrix * ortho;

        defaultShader.useProgram();


        // Projection demo cube
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        defaultShader.setUniform("model", demo_model);

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        if (lmb) {
            glfwGetCursorPos(window, &cursor_x, &cursor_y);
            cursor_vec(cursor_x, cursor_y);
        } else {
            clear(cursor_positions);
        }

        inv_model = glm::inverse(glm::mat3(model));

        rotate_vec = calc_rot_vec(); // rotation axis (absolute)
        rotate_vec = inv_model * rotate_vec; // rotation axis (relative)

        position = calc_translation(); // positon (absolute)
        position = inv_model * position; // position (relative)

        scale = calc_scale(scroll); // scale
        rotate_angle = calc_rot_angle(); // rotation angle

        absscale *= scale;

        model = glm::translate(model, position);
        if (rotate_angle > 0) model = glm::rotate(model, rotate_angle, rotate_vec);
        model = glm::scale(model, glm::vec3(scale));

        wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) :
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        defaultShader.setUniform("model", model);
        defaultShader.setUniform("proj", proj);

        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 20 * (slices + 1));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    delete[] surface;
    delete[] cube;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

glm::mat4 horizontalIsometry() {
    glm::mat4 T1, T2, T3;

    T1[2][2] = -1.0f;
    T2[2][0] = -1.0f * sqrt(2) / 2;
    T3[2][1] = -1.0f * sqrt(2) / 2;

    return T1 * T2 * T3;
}

glm::vec2 pixel_to_window(double x, double y) {
    glm::vec2 screen;

    screen.x = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
    screen.y = (WINDOW_HEIGHT / 2 - y ) / (WINDOW_HEIGHT / 2);

    return screen;
}

// ===== Callbacks ======
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        set_key(key, true);

        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_SPACE) wireframe = !wireframe;

        if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {

            delete[] surface;

            if (key == GLFW_KEY_UP) {
                slices++;
            } else if (slices > 3) {
                slices--;
            }

            surface = gen_surface(slices);

            glBindVertexArray(VAO[1]);
            glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
            glBufferData(GL_ARRAY_BUFFER, (slices + 1) * 10 * 6 * sizeof(GLfloat), surface, GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }

    if (action == GLFW_RELEASE) {
        set_key(key, false);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    float ratio = (float) width / (float) height;
    ortho = glm::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        lmb = (action == GLFW_PRESS);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    scroll = (yoffset > 0) ? 1 : -1;
}

// ===== Cursor calculations =====
void cursor_vec(double xpos, double ypos) {
    cursor_positions.push(pixel_to_window(xpos, ypos));
    while (cursor_positions.size() > MAX_QUEUE) { cursor_positions.pop(); }

    if (cursor_positions.size() > 1) {
        glm::vec2 cpf = cursor_positions.front();
        glm::vec2 cpb = cursor_positions.back();

        glm::vec2 dir = cpf - cpb;
        dir /= absscale * 5.0f;

        set_cdv(dir);
    }
}

void clear(std::queue<glm::vec2> &q) {
    std::queue<glm::vec2> empty;
    std::swap(q, empty);
}
