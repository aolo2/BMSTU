#include "Object/Object.h"
#include "Shader/Shader.h"
#include "Movement/Movement.h"

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
    Object volume(Shape::CUBE, GL_LINE);

    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    Shader allWhite("GLSL/allwhite.vert", "GLSL/allwhite.frag");

    glm::mat4 proj, view, model, model2;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); // Cavalier();
    
    glm::vec3 current_pos(0.0f), lightPos;
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    
    std::vector<glm::vec3> collision_data = item.get_3d_collision();
    std::vector<glm::vec3> world_collision(collision_data);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
                
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (int i = 0; i < world_collision.size(); i++) {
            world_collision[i] = glm::vec3(model * glm::vec4(collision_data[i], 1.0f));
        }
        
        
        current_pos = calculate_position(current_pos, world_collision, 0.017f); // now - then (in seconds)
        
        model = glm::translate(glm::mat4(), current_pos);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        // model = glm::rotate(glm::mat4(), (float) glfwGetTime() * 0.837f, glm::vec3(1.0f, 0.0f, 0.0f));

        // object
        defaultShader.useProgram();
        defaultShader.setUniform("model", model);
        defaultShader.setUniform("view", view);
        defaultShader.setUniform("proj", proj);
        defaultShader.setUniform("lightPos", lightPos);
        
        item.render();
        
        lightPos = glm::vec3(0.5f * sin(glfwGetTime()), 0.5f * cos(glfwGetTime()), 0.5f);
        model2 = glm::translate(glm::mat4(), lightPos);
        model2 = glm::scale(model2, glm::vec3(0.05f));
        
        // light (white cube)
        allWhite.useProgram();
        allWhite.setUniform("model", model2);
        allWhite.setUniform("view", view);
        allWhite.setUniform("proj", proj);
        
        light.render();
        
        
        // volume (bounce surfaces)
        model2 = glm::scale(glm::mat4(), glm::vec3(1.0f));
        allWhite.setUniform("model", model2);
        
        volume.render();
        
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