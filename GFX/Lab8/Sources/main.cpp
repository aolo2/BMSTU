#define GLEW_STATIC
#include <GL/glew.h>

#include "../Includes/Object.h"
#include "../Includes/Shader.h"
#include "../Includes/Callbacks.h"

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "OpenGL Lab 8", nullptr, nullptr);
    
    glfwSetCursorPosCallback(window, Callbacks::cursor_pos_callback);
    glfwSetKeyCallback(window, Callbacks::key_callback);
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
    
    const Utils::Object room;
    
    Utils::Shader flatShader("GLSL/default.vert", "GLSL/color.frag");

    glm::mat4 model, view, proj;
    model = glm::scale(glm::mat4(), glm::vec3(1.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    flatShader.useProgram();
    flatShader.setUniform("color", glm::vec3(1.0f));
    flatShader.setUniform("model", model);
    flatShader.setUniform("view", view);
    flatShader.setUniform("proj", proj);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        flatShader.useProgram();
        flatShader.setUniform("model", glm::rotate(glm::mat4(), (float) glfwGetTime(), Utils::Y_POS));
        room.render();
        
        glfwSwapBuffers(window);
    }
        
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}