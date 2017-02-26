#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Shapes/Shapes.h"

#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <vector>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define ASTEROIDS_COUNT 100
#define RADIUS 35

bool keys[1024];
Camera camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void process_keyboard(GLFWwindow *window);

bool cull = false;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL 4.3 Test Scene", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader light("GLSL/light.vert", "GLSL/light.frag");

    std::vector<glm::vec3> planet;
    std::vector<int> planet_i;

    gen_sphere(planet, planet_i, 10.0f, 60, 60);

    std::vector<glm::vec3> asteroids[ASTEROIDS_COUNT];
    std::vector<int> asteroids_i[ASTEROIDS_COUNT];
    for (int i = 0; i < 100; i++) {
        gen_random_shit(asteroids[i], asteroids_i[i], i);
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // VAO's VBO's EBO's
    GLuint VAO, VBO[101], EBO[101];
    glGenVertexArrays(1, &VAO);
    glGenBuffers(101, VBO);
    glGenBuffers(101, EBO);

    glm::mat4 model, view, proj;
    proj = glm::perspective(45.0f, (GLfloat) WINDOW_WIDTH / (GLfloat) WINDOW_HEIGHT, 0.1f, 10000.0f);

    glClearColor(0.1f, 0.1f, 0.115f, 1.0f);
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        process_keyboard(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        light.useProgram();
        view = camera.viewMatrix();

        model = glm::mat4();

        light.setUniform("model", model);
        light.setUniform("view", view);
        light.setUniform("proj", proj);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, planet.size() * sizeof(GLfloat) * 3, planet.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, planet_i.size() * sizeof(GL_UNSIGNED_INT), planet_i.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawElements(GL_TRIANGLES, planet_i.size(), GL_UNSIGNED_INT, 0);

        GLfloat shift = (GLfloat) glfwGetTime();

        for (int i = 0; i < ASTEROIDS_COUNT; i++) {
            float angle = 2 * PI / ASTEROIDS_COUNT;
            angle *= i;

            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
            glBufferData(GL_ARRAY_BUFFER, asteroids[i].size() * sizeof(GLfloat) * 3, asteroids[i].data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, asteroids_i[i].size() * sizeof(GL_UNSIGNED_INT), asteroids_i[i].data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            model = glm::translate(glm::mat4(), glm::vec3(RADIUS * cosf(angle + shift / 50), 0.0f, RADIUS * sinf(angle + shift/ 50)));
            light.setUniform("model", model);

            glDrawElements(GL_TRIANGLES, asteroids_i[i].size(), GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

bool firstCall = true;
double lastX, lastY;
void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstCall) {
        lastX = xpos;
        lastY = ypos;
        firstCall = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.processCursor(xoffset, yoffset, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        if (key == GLFW_KEY_ENTER) cull = !cull;
    }

    if (action == GLFW_RELEASE) { keys[key] = false; }
}

void process_keyboard(GLFWwindow *window) {
    if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);
    if (keys[GLFW_KEY_W]) { camera.processMovement(GLFW_KEY_W); }
    if (keys[GLFW_KEY_A]) { camera.processMovement(GLFW_KEY_A); }
    if (keys[GLFW_KEY_S]) { camera.processMovement(GLFW_KEY_S); }
    if (keys[GLFW_KEY_D]) { camera.processMovement(GLFW_KEY_D); }
}
