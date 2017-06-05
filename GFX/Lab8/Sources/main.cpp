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
    
    Utils::Shader flatShader("GLSL/default.vert", "GLSL/color.frag");
    Utils::Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    
    std::vector<Utils::Object> scene;
    
    Utils::Object room(Utils::Shape::CUBE, &flatShader);
    Utils::Object cube(Utils::Shape::CUBE_GRAY, &flatShader);
    Utils::Object light(Utils::Shape::CUBE, &defaultShader);
    
    scene.push_back(room);
    scene.push_back(cube);
    scene.push_back(light);

    glm::vec3 lightpos(0.0f, 0.48f, 0.0f), cubepos(0.0f);
    glm::mat4 view, proj, roommodel, lightmodel, cubemodel;
    
    lightmodel = glm::translate(glm::mat4(), lightpos);
    lightmodel = glm::scale(lightmodel, glm::vec3(0.3f, 0.02f, 0.3f));
    
    cubemodel = glm::translate(glm::mat4(), cubepos);
    cubemodel = glm::scale(cubemodel, glm::vec3(0.4f));
    
    roommodel = glm::scale(glm::mat4(), glm::vec3(1.0f));
    
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    view = glm::rotate(view, glm::radians(90.0f), Utils::Y_POS);
    // view = glm::scale(view, glm::vec3(0.05f));
    proj = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
    
    flatShader.useProgram();
    flatShader.setUniform("color", glm::vec3(1.0f));
    flatShader.setUniform("model", roommodel);
    flatShader.setUniform("view", view);
    flatShader.setUniform("proj", proj);
    
    defaultShader.useProgram();
    defaultShader.setUniform("model", lightmodel);
    defaultShader.setUniform("view", view);
    defaultShader.setUniform("proj", proj);
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        cubemodel = glm::rotate(cubemodel, glm::radians(1.0f), Utils::Y_POS);
        // room.smodel(roommodel);
        // cube.smodel(cubemodel);
        // light.smodel(lightmodel);
        
        flatShader.useProgram();
        // flatShader.setUniform("model", glm::rotate(glm::mat4(), (float) glfwGetTime(), Utils::Y_POS));
        flatShader.setUniform("Lightpos", lightpos);
        
        flatShader.setUniform("model", roommodel);
        flatShader.setUniform1i("invert", 1);
        glFrontFace(GL_CCW);
        room.render();

        flatShader.setUniform("model", cubemodel);
        flatShader.setUniform1i("invert", 0);
        glFrontFace(GL_CW);
        cube.render();
        

        defaultShader.useProgram();
        glFrontFace(GL_CCW);
        light.render();
        
        glfwSwapBuffers(window);
    }
        
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
