#include "Object/Object.h"
#include "Shader/Shader.h"
#include "Scene/Scene.h"
#include "Movement/Movement.h"

#include <GLFW/glfw3.h>

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

bool textured(false), paused(false), keys[1024];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 6", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    // glLineWidth(2.0f);
    // glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    Object item(Shape::SPIN_SURFACE), light(Shape::CUBE);
    Object volume(Shape::CUBE, GL_LINE);

    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    Shader allWhite("GLSL/allwhite.vert", "GLSL/allwhite.frag");

    glm::mat4 proj, view, model, model2;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f); // Cavalier();
    
    glm::vec3 current_pos(0.0f), lightPos;
    
    float scale_factor(0.4f);
    float collision_radius(item.get_collision_radius());
    
    
    // Load scene
    Scene scene = load_scene("Scene1.scn");
    
    set_start_dir(scene.start_dir);
    set_speed(scene.speed);
    set_light_speed(scene.light_speed);
    
    textured = scene.textured;
    paused = scene.paused;
    current_pos = scene.start_pos;
    lightPos = scene.light_pos;
    
    defaultShader.useProgram();
    defaultShader.setUniform("diffuse", scene.diffuse);
    
    // Start the timer
    float start = scene.time_elapsed, then = start + glfwGetTime(), now;
    unsigned int frames = 0;
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
               
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        now = start + glfwGetTime();
        
        // item pos
        if (!paused) {
            current_pos = calculate_position(current_pos, scale_factor * collision_radius, now - then);
        }
        
        // light pos
        lightPos = calculate_light_positon(lightPos, keys, now - then);
        
        then = now;
    	scene.time_elapsed = now;

        model = glm::translate(glm::mat4(), current_pos);
        model = glm::rotate(model, glm::radians(42.0f), glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scale_factor));
    
        defaultShader.useProgram();
        defaultShader.setUniform("model", model);
        defaultShader.setUniform("view", view);
        defaultShader.setUniform("proj", proj);
        defaultShader.setUniform("lightPos", lightPos);
        defaultShader.setUniform1i("textured", (textured ? 1 : 0));
        defaultShader.setUniform1i("albedo", 0);
        
        item.render();


        // light (white cube)
        model2 = glm::translate(glm::mat4(), lightPos);
        model2 = glm::scale(model2, glm::vec3(0.05f));

        allWhite.useProgram();
        allWhite.setUniform("model", model2);
        allWhite.setUniform("view", view);
        allWhite.setUniform("proj", proj);
        
        light.render();
        
        
        // volume (bounce surfaces)
        model2 = glm::scale(glm::mat4(), glm::vec3(0.5f));
        allWhite.setUniform("model", model2);
        
        volume.render();
        
        glfwSwapBuffers(window);
	    ++frames;
    }

    // std::cout << "Average frame time: " << (glfwGetTime() - start2) / frames * 1000.0f << "ms" << std::endl;

    scene.paused = paused;
    scene.textured = textured;
    scene.light_pos = lightPos;
    scene.start_pos = current_pos;
    scene.light_speed = get_light_speed();
    scene.start_dir = get_current_dir();
    
    save_scene(scene);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_SPACE) textured = !textured;
        if (key == GLFW_KEY_PAUSE) paused = !paused;
        
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
    
    keys[1001] = mods;
}