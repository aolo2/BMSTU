#include "Shader.h"
#include "Camera.h"
#include "Shapes.h"

// GLFW (mouse and keyboard events, windows)
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

bool keys[1000];
GLFWwindow *window;
Camera cam(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

glm::vec2 px_to_screen(int x, int y) {
    glm::vec2 screen;
    
    screen.x = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
    screen.y = (WINDOW_HEIGHT / 2 - y ) / (WINDOW_HEIGHT / 2);
    
    return screen;
}

void processKeyboard() {
    if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);

    if (keys[GLFW_KEY_W]) { cam.processMovement(GLFW_KEY_W); }
    if (keys[GLFW_KEY_A]) { cam.processMovement(GLFW_KEY_A); }
    if (keys[GLFW_KEY_S]) { cam.processMovement(GLFW_KEY_S); }
    if (keys[GLFW_KEY_D]) { cam.processMovement(GLFW_KEY_D); }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
    glfwSetKeyCallback(window, key_callback);  
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    std::vector<glm::vec3> vertices_vector, light_vertices;
    std::vector<int> index_vector, light_index;
    
    gen_sphere(vertices_vector, index_vector, 0.5f, 25, 25);
    gen_cube(light_vertices, light_index, 0.15f);
    
    GLuint VBO, VAO, EBO;
    GLuint lightVBO, lightEBO;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);

    Shader shaderProgram("default.vert", "default.frag");
    Shader lightShader("light.vert", "light.frag");
    
    glm::mat4 model, view, proj;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    DirLight dirLight;
    PointLight pointLight;
    ConeLight coneLight;
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        GLfloat time = (GLfloat) glfwGetTime();
        
        /* ===========DRAW HERE============= */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        processKeyboard();
        
        shaderProgram.useProgram();
        
        // Transfomation matrices
        model = glm::mat4();
        view = cam.viewMatrix();
        proj = glm::perspective(45.0f, (GLfloat) WINDOW_WIDTH / (GLfloat) WINDOW_HEIGHT, 0.1f, 100.0f);
        
        shaderProgram.setUniform("model", model);
        shaderProgram.setUniform("view", view);
        shaderProgram.setUniform("proj", proj);
        
        shaderProgram.setUniform("camPos", cam.getPosition());
        coneLight.direction = cam.getFront();
        pointLight.position = glm::vec3(3 + sin(time), 0.6f, cos(time));

        // Lights
        shaderProgram.setUniform("dirLight", dirLight);
        shaderProgram.setUniform("pointLight", pointLight);
        shaderProgram.setUniform("coneLight", coneLight);
        
        
        // Bind data and indices and draw
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_vector.size() * sizeof(GLfloat) * 3, vertices_vector.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_vector.size() * sizeof(GL_UNSIGNED_INT), index_vector.data(), GL_STATIC_DRAW); 
        
        for (int i = 0; i < 5; i++) {
            model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
            shaderProgram.setUniform("model", model);
            glDrawElements(GL_TRIANGLES, index_vector.size(), GL_UNSIGNED_INT, 0);
        }
        // ===============================
        
        
        lightShader.useProgram();
        model = glm::translate(glm::mat4(), pointLight.position);
        model = glm::scale(model, glm::vec3(0.5f));
        
        lightShader.setUniform("model", model);
        lightShader.setUniform("view", view);
        lightShader.setUniform("proj", proj);
        lightShader.setUniform("lightColor", pointLight.color);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, light_vertices.size() * sizeof(GLfloat) * 3, light_vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, light_index.size() * sizeof(GL_UNSIGNED_INT), light_index.data(), GL_STATIC_DRAW); 
        glDrawElements(GL_TRIANGLES, light_index.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        /* ================================= */
                
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

bool lmb_pressed, rmb_pressed, firstCall = true;
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

    cam.processCursor(xoffset, yoffset, true);
}    

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) { lmb_pressed = true; }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) { rmb_pressed = true; }
    } else if (action == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) { lmb_pressed = false;  }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) { rmb_pressed = false; }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) { keys[key] = true; }
    if (action == GLFW_RELEASE) { keys[key] = false; }
    
    cam.processMovement(key);
}