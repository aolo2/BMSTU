#include "Shader/Shader.h"
#include "Utils/Utils.h"

#include <GLFW/glfw3.h>

std::vector<glm::vec2> vertices;
std::vector<glm::vec2> vertices_cutter;
std::vector<glm::vec2> vertices_clipped;
std::vector<glm::vec2> bigdata;
std::vector<std::vector<Utils::Vertex>> c;

GLuint VAO[2], VBO[2];
GLuint BIGVAO, BIGVBO;

bool clipped = false;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 5", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glLineWidth(2.0f);
    glEnable(GL_MULTISAMPLE);

    glGenVertexArrays(1, &BIGVAO);
    glGenVertexArrays(1, &BIGVBO);



    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);

    Utils::rebind(VAO[0], VBO[0], vertices);
    Utils::rebind(VAO[1], VBO[1], vertices_cutter);

    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");

    std::cout << "\nUsage:\n------\n";
    std::cout << "LMB       - add polygon vertex\n";
    std::cout << "RMB       - add cutter vertex\n";
    std::cout << "SPACE     - do a cut\n";
    std::cout << "DELETE    - clear polygon vetices\n";
    std::cout << "BACKSPACE - clear cutter vertices\n";
    std::cout << "------\n";

    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        defaultShader.useProgram();

        defaultShader.setUniform("color", glm::vec3(1.0f));

        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_LINE_LOOP, 0, vertices.size());

        if (clipped) {
            defaultShader.setUniform("color", glm::vec3(0.5f, 0.5f, 0.85f));
            glBindVertexArray(BIGVAO);
            unsigned int offset = 0;
            for (const auto &vec : c) {
                glDrawArrays(GL_LINE_LOOP, offset, vec.size());
                offset += vec.size();
            }
            glBindVertexArray(0);
        } else {
            defaultShader.setUniform("color", glm::vec3(1.0f, 0.5f, 0.0f));
            glBindVertexArray(VAO[1]);
            glDrawArrays(GL_LINE_LOOP, 0, vertices_cutter.size());
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(2, VBO);
    glDeleteVertexArrays(2, VAO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// ===== Callbacks ======
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_DELETE:
                vertices.clear();

                clipped = false;

                Utils::rebind(VAO[0], VBO[0], vertices);
                break;
            case GLFW_KEY_BACKSPACE:
                vertices_cutter.clear();

                clipped = false;

                Utils::rebind(VAO[1], VBO[1], vertices_cutter);
                break;
            case GLFW_KEY_SPACE:

                c = Utils::clip(vertices_cutter, vertices);

                for (const auto &vec : c) {
                    for (const auto &vert : vec) {
                        bigdata.push_back(vert.coords);
                    }
                }

                clipped = true;
                Utils::rebind(BIGVAO, BIGVBO, bigdata);
                // Utils::rebind(VAO[0], VBO[0], vertices);
                // Utils::rebind(VAO[1], VBO[1], vertices_clipped);

                break;
        }
    }

    if (action == GLFW_RELEASE) { }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        glm::vec2 vertex = Utils::window_to_relative(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            vertices.push_back(vertex);
            Utils::rebind(VAO[0], VBO[0], vertices);
        } else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
            vertices_cutter.push_back(vertex);
            Utils::rebind(VAO[1], VBO[1], vertices_cutter);
        }
    }
}
