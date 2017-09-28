#include "Includes/Shader.h"
#include "Includes/Drawable.h"
#include "Includes/Callbacks.h"

const unsigned int WINDOW_WIDTH = 1280;
const unsigned int WINDOW_HEIGHT = 720;
const float aspect_ratio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, callback::key_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    utils::shader color_shader("GLSL/pass.vert", "GLSL/color.frag");
    utils::drawable room("Models/cornell_box.obj");
    
    // 278.0f, 0.0f, 279.5f -- light source center

    glm::mat4 proj, view, model;
    model = glm::scale(glm::mat4(), glm::vec3(1.0f));
    view = glm::translate(glm::mat4(), glm::vec3(-278.0f, -273.0f, -800.0f));
    proj = glm::perspective(glm::radians(45.0f), aspect_ratio, 0.1f, 10000.0f);

    color_shader.use_program();
    color_shader.set_uniform<glm::mat4>("proj", proj);
    color_shader.set_uniform<glm::mat4>("view", view);
    color_shader.set_uniform<glm::mat4>("model", model);

    while (glfwWindowShouldClose(window) == 0) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        room.render();

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}