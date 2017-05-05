#include "Object/Object.h"
#include "Shader/Shader.h"
#include "Shapes/Shapes.h"

#include <GLFW/glfw3.h>

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

glm::mat4 Cavalier();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // fix!

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 6", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Object item(Shape::SPIN_SURFACE), light(Shape::CUBE);

    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    Shader allWhite("GLSL/allwhite.vert", "GLSL/allwhite.frag");

    glm::mat4 customProjMatrix, model;
    customProjMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); // Cavalier();
    
    glm::vec3 lightPos;
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        model = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -2.0f));
        model = glm::rotate(model, (float) glfwGetTime() * 0.837f, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));

        // object
        defaultShader.useProgram();
        defaultShader.setUniform("model", model);
        defaultShader.setUniform("proj", customProjMatrix);
        defaultShader.setUniform("lightPos", lightPos);
        
        item.render();
        
        model = glm::translate(glm::mat4(), lightPos);
        model = glm::scale(model, glm::vec3(0.1f));
        lightPos = glm::vec3(0.5f * sin(glfwGetTime()), 0.5f * cos(glfwGetTime()), -1.0f);
        
        // light (white cube)
        allWhite.useProgram();
        allWhite.setUniform("model", model);
        allWhite.setUniform("proj", customProjMatrix);
        
        light.render();
        
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

glm::mat4 Cavalier() {
    glm::mat4 T2;

    T2[2][0] = sqrt(2) / -2.0f;
    T2[2][1] = sqrt(2) / -2.0f;

    return T2;
}