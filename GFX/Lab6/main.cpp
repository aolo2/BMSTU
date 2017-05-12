#include "Object/Object.h"
#include "Shader/Shader.h"
#include "Movement/Movement.h"

#include <GLFW/glfw3.h>

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

int textured = 1;

bool keys[1024];

struct Scene {
    glm::vec3 start_pos;
    glm::vec3 start_dir;
    glm::vec3 light_pos;
    
    float time_elapsed;
    float diffuse;
    float speed;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void process_keyboard(glm::vec3 &lightPos);
void save_scene(const Scene &scene);

Scene load_scene(const std::string &path);
glm::mat4 Cavalier();

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 6", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
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
    Scene scene = load_scene("Scenes/Scene1.scn");
    
    set_start_dir(scene.start_dir);
    set_speed(scene.speed);
    
    current_pos = scene.start_pos;
    lightPos = scene.light_pos;
    
    defaultShader.useProgram();
    defaultShader.setUniform("diffuse", scene.diffuse);
    
    // Start the timer
    float start = scene.time_elapsed;
    float then = start + glfwGetTime(), now;
    
    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        process_keyboard(lightPos);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        now = start + glfwGetTime();
        
        // item
        current_pos = calculate_position(current_pos, scale_factor * collision_radius, now - then);
        then = now;
        scene.time_elapsed = now;
        
        model = glm::translate(glm::mat4(), current_pos);
        model = glm::rotate(model, start + now, glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scale_factor));

        defaultShader.useProgram();
        defaultShader.setUniform("model", model);
        defaultShader.setUniform("view", view);
        defaultShader.setUniform("proj", proj);
        defaultShader.setUniform("lightPos", lightPos);
        defaultShader.setUniform1i("textured", textured);
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
    }

    scene.light_pos = lightPos;
    scene.start_pos = current_pos;
    scene.start_dir = get_current_dir();
    
    save_scene(scene);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void process_keyboard(glm::vec3 &lightPos) {
    if (keys[GLFW_KEY_UP]) {
        if (keys[1001] & GLFW_MOD_SHIFT) {
            lightPos -= glm::vec3(0.0f, 0.0f, 0.1f);   
        } else {
            lightPos += glm::vec3(0.0f, 0.1f, 0.0f);   
        }
    }
    
    if (keys[GLFW_KEY_DOWN]) {
        if (keys[1001] & GLFW_MOD_SHIFT) {
            lightPos += glm::vec3(0.0f, 0.0f, 0.1f);   
        } else {
            lightPos -= glm::vec3(0.0f, 0.1f, 0.0f);
        }
    }
    
    if (keys[GLFW_KEY_LEFT]) {
        lightPos -= glm::vec3(0.1f, 0.0f, 0.0f);   
    }
    
    if (keys[GLFW_KEY_RIGHT]) {
        lightPos += glm::vec3(0.1f, 0.0f, 0.0f);   
    }
}

void save_scene(const Scene &scene) {
    std::ofstream f;
    
    std::cout << "Saving scene..." << std::endl;
    
    f.open("Scenes/Scene1.scn");
    
    f << "lab_6\n\n";
    f << "start_pos = " << scene.start_pos.x << " " << scene.start_pos.y << " " << scene.start_pos.z << std::endl;
    f << "start_dir = " << scene.start_dir.x << " " << scene.start_dir.y << " " << scene.start_dir.z << std::endl;
    f << std::endl;
    f << "diffuse = " << scene.diffuse << std::endl;
    f << "time_elapsed = " << scene.time_elapsed << std::endl;
    f << "speed = " << scene.speed << std::endl;
    f << "light_pos = " << scene.light_pos.x << " " << scene.light_pos.y << " " << scene.light_pos.z << std::endl;
    
    f.close();
    
    std::cout << "Done." << std::endl;
}

Scene load_scene(const std::string &path) {
    std::ifstream f(path);
    
    unsigned int line_number(0);
    std::string line;

    // read scene title
    std::getline(f, line);
    std::cout << "Loading scene \'" << line << "\'..." << std::endl;
    ++line_number;
    
    Scene scene;
    
    // start_pos
    while (std::getline(f, line)) {
        
        std::string var_name(line.substr(0, line.find(" =")));
        std::string values;
        std::string::size_type sz(0), sz2(0);
        
        float x, y, z;
        
        ++line_number;

        if (var_name == "start_pos") {
            values = line.substr(line.find("=") + 2);
            x = std::stof(values, &sz);
            y = std::stof(values.substr(sz), &sz2);
            z = std::stof(values.substr(sz + sz2));
            
            scene.start_pos = glm::vec3(x, y, z);                            
        } else if (var_name == "start_dir") {
            values = line.substr(line.find("=") + 2);
            x = std::stof(values, &sz);
            y = std::stof(values.substr(sz), &sz2);
            z = std::stof(values.substr(sz + sz2));
            
            scene.start_dir = glm::vec3(x, y, z);
        } else if (var_name == "light_pos") {
             values = line.substr(line.find("=") + 2);
             x = std::stof(values, &sz);
             y = std::stof(values.substr(sz), &sz2);
             z = std::stof(values.substr(sz + sz2));
             
             scene.light_pos = glm::vec3(x, y, z);
        } else if (var_name == "diffuse") {
             values = line.substr(line.find("=") + 2);
             x = std::stof(values, &sz);
             
             scene.diffuse = x;
        } else if (var_name == "speed") {
             values = line.substr(line.find("=") + 2);
             x = std::stof(values, &sz);
             
             scene.speed = x;
        } else if (var_name == "time_elapsed") {
             values = line.substr(line.find("=") + 2);
             x = std::stof(values, &sz);
              
             scene.time_elapsed = x;
        } else if (var_name.size() > 0) {
            std::cout << "Unknown variable \'" << var_name << "\' at line " << line_number << std::endl;
        }
    }
            
    std::cout << "Done." << std::endl;
    
    return scene;
}

glm::mat4 Cavalier() {
    glm::mat4 T2;

    T2[2][0] = sqrt(2) / -2.0f;
    T2[2][1] = sqrt(2) / -2.0f;

    return T2;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (key == GLFW_KEY_SPACE) textured = (textured == 1) ? 0 : 1;
        
        keys[key] = true;
    } else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
    
    keys[1001] = mods;
}