#define GLEW_STATIC
#include <GL/glew.h>

#include "../Includes/Object.h"
#include "../Includes/Shader.h"

#include <GLFW/glfw3.h>

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 8);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "OpenGL Lab 8", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    Utils::Object cube;
    Utils::Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    
    glm::mat4 model, view, proj;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    defaultShader.useProgram();
    defaultShader.setUniform("view", view);
    defaultShader.setUniform("proj", proj);
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        model = glm::scale(glm::mat4(), glm::vec3(0.5f));
        model = glm::rotate(model, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        
        defaultShader.useProgram();
        defaultShader.setUniform("model", model);
        
        cube.render();
        
        glfwSwapBuffers(window);
    }
        
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}