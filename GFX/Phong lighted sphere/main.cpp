#include "Shader.h"
#include "Camera.h"
#include <vector>

// GLFW (mouse and keyboard events, windows)
#include <GLFW/glfw3.h>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define PI 3.1415926f

struct DirLight {
    glm::vec3 direction, ambient, diffuse, specular, color;
};  

struct PointLight {    
    glm::vec3 position, ambient, diffuse, specular, color;
    glm::vec3 k0k1k2;  
};  

struct ConeLight {
    glm::vec3 color, direction, k0k1k2;
    float hardAngle, softAngle, fadeK, diffuse;
};

bool keys[1000];
GLFWwindow *window;
Camera cam(glm::vec3(2.5f, -2.5f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);

glm::vec2 px_to_screen(int x, int y) {
    glm::vec2 screen;
    
    screen.x = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
    screen.y = (WINDOW_HEIGHT / 2 - y ) / (WINDOW_HEIGHT / 2);
    
    return screen;
}

void gen_sphere(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector, 
                float radius, int slices, int stacks) {
                    
    for (int i = 0; i <= stacks; i++) {
        float V = i / (float) stacks;
        float phi = V * PI;
    
        // Loop Through Slices
        for (int j = 0; j <= slices; j++) {
            float U = (float) j / (float) slices;
            float theta = U * 2 * PI;
    
            // Calc The Vertex Positions
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);
    
            // Push Back Vertex Data
            vertices_vector.push_back(glm::vec3(x, y, z) * radius);
        }
    }
    
    
    // Calc The Index Positions
    for (int i = 0; i < slices * stacks + slices; i++){
        index_vector.push_back(i);
        index_vector.push_back(i + slices + 1);
        index_vector.push_back(i + slices);
    
        index_vector.push_back(i + slices + 1);
        index_vector.push_back(i);
        index_vector.push_back(i + 1);
    }
}

void gen_cube(std::vector<glm::vec3> &vertices_vector, std::vector<int> &index_vector, float half_side) {
    vertices_vector.push_back(glm::vec3(-1 * half_side, half_side, -1 * half_side)); // 0
    vertices_vector.push_back(glm::vec3(half_side, half_side, -1 * half_side)); // 1
    vertices_vector.push_back(glm::vec3(-1 * half_side, half_side, half_side)); // 2
    vertices_vector.push_back(glm::vec3(half_side, half_side, half_side)); // 3
    
    vertices_vector.push_back(glm::vec3(-1 * half_side, -1 * half_side, -1 * half_side)); // 4
    vertices_vector.push_back(glm::vec3(half_side, -1 * half_side, -1 * half_side)); // 5
    vertices_vector.push_back(glm::vec3(-1 * half_side, -1 * half_side, half_side)); // 6
    vertices_vector.push_back(glm::vec3(half_side, -1 * half_side, half_side)); // 7
    
    /*
    
    0----1
    |    |
    2----3
    
    
    4----5
    |    |
    6----7
    
    */
    
    // top face
    index_vector.push_back(0);
    index_vector.push_back(1);
    index_vector.push_back(2);
    index_vector.push_back(1);
    index_vector.push_back(2);
    index_vector.push_back(3);
    
    
    // bottom face
    index_vector.push_back(4);
    index_vector.push_back(5);
    index_vector.push_back(6);
    index_vector.push_back(5);
    index_vector.push_back(6);
    index_vector.push_back(7);
    
    
    // front face
    index_vector.push_back(2);
    index_vector.push_back(3);
    index_vector.push_back(6);
    index_vector.push_back(3);
    index_vector.push_back(6);
    index_vector.push_back(7);
    
    
    // right face
    index_vector.push_back(1);
    index_vector.push_back(3);
    index_vector.push_back(7);
    index_vector.push_back(1);
    index_vector.push_back(5);
    index_vector.push_back(7);
    
    
    // back face
    index_vector.push_back(0);
    index_vector.push_back(1);
    index_vector.push_back(4);
    index_vector.push_back(1);
    index_vector.push_back(4);
    index_vector.push_back(5);
    
    
    // left face
    index_vector.push_back(0);
    index_vector.push_back(2);
    index_vector.push_back(4);
    index_vector.push_back(2);
    index_vector.push_back(4);
    index_vector.push_back(6);
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
    glDepthFunc(GL_LEQUAL);
    
    glfwSetKeyCallback(window, key_callback);  
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    std::vector<glm::vec3> vertices_vector, light_vertices;
    std::vector<int> index_vector, light_index;
    
    gen_sphere(vertices_vector, index_vector, 0.5f, 20, 20);
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
    glClearColor(0.02f, 0.02f, 0.024f, 1.0f);
    
    DirLight dirLight;
    dirLight.direction = glm::vec3(-1.0f, -1.0f, -2.0f);
    dirLight.ambient = glm::vec3(0.02f);
    dirLight.diffuse = glm::vec3(0.2f);
    dirLight.specular = glm::vec3(0.2f);
    dirLight.color = glm::vec3(1.0f, 1.0f, 0.8f);
    
    PointLight pointLight;
    pointLight.position = glm::vec3(5.0f, -4.0f, 2.5f);
    pointLight.ambient = glm::vec3(0.02f);
    pointLight.diffuse = glm::vec3(0.2f);
    pointLight.specular = glm::vec3(0.9f);
    pointLight.color = glm::vec3(1.0f);
    pointLight.k0k1k2 = glm::vec3(1.0f, 0.7f, 1.8f);
    
    ConeLight coneLight;
    coneLight.color = glm::vec3(1.0f);
    coneLight.hardAngle = cos(glm::radians(25.5f));
    coneLight.softAngle = cos(glm::radians(37.0f));
    coneLight.diffuse = 0.9f;
    coneLight.k0k1k2 = glm::vec3(1.0f, 0.25f, 0.34f);
    
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
        
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram.getProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));
        
        // Directional light
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "dirLight.direction"), dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "dirLight.ambient"), dirLight.ambient.x, dirLight.ambient.y, dirLight.ambient.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "dirLight.diffuse"), dirLight.diffuse.x, dirLight.diffuse.y, dirLight.diffuse.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "dirLight.specular"), dirLight.specular.x, dirLight.specular.y, dirLight.specular.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "dirLight.color"), dirLight.color.x, dirLight.color.y, dirLight.color.z);
        
        
        // Point light
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "pointLight.position"), pointLight.position.x, pointLight.position.y, pointLight.position.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "pointLight.ambient"), pointLight.ambient.x, pointLight.ambient.y, pointLight.ambient.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "pointLight.diffuse"), pointLight.diffuse.x, pointLight.diffuse.y, pointLight.diffuse.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "pointLight.specular"), pointLight.specular.x, pointLight.specular.y, pointLight.specular.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "pointLight.color"), pointLight.color.x, pointLight.color.y, pointLight.color.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "pointLight.k0k1k2"), pointLight.k0k1k2.x, pointLight.k0k1k2.y, pointLight.k0k1k2.z);
        
        
        // Camera position
        glm::vec3 camPos = cam.getPosition();
        coneLight.direction = cam.getFront();
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "camPos"), camPos.x, camPos.y, camPos.z);
        
        // Cone light
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.direction"), coneLight.direction.x, coneLight.direction.y, coneLight.direction.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.color"), coneLight.color.x, coneLight.color.y, coneLight.color.z);
        glUniform3f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.k0k1k2"), coneLight.k0k1k2.x, coneLight.k0k1k2.y, coneLight.k0k1k2.z);
        glUniform1f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.hardAngle"), coneLight.hardAngle);
        glUniform1f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.softAngle"), coneLight.softAngle);
        glUniform1f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.softAngle"), coneLight.softAngle);
        glUniform1f(glGetUniformLocation(shaderProgram.getProgram(), "coneLight.diffuse"), coneLight.diffuse);
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices_vector.size() * sizeof(GLfloat) * 3, vertices_vector.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_vector.size() * sizeof(GL_UNSIGNED_INT), index_vector.data(), GL_STATIC_DRAW); 

        glm::mat4 baseModel = model;
        GLint modelLoc = glGetUniformLocation(shaderProgram.getProgram(), "model");
        for (int i = 0; i < 5; i++) {
            model = glm::translate(baseModel, glm::vec3(0.0f, -1.5f * i, 0.0f));
            for (int j = 0; j < 5; j++) {
                model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));        
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glDrawElements(GL_TRIANGLES, index_vector.size(), GL_UNSIGNED_INT, 0);
            }
        }
        
        
        lightShader.useProgram();
        model = glm::translate(glm::mat4(), glm::vec3(pointLight.position));
        // model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.getProgram(), "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightShader.getProgram(), "proj"), 1, GL_FALSE, glm::value_ptr(proj));

        glUniform3f(glGetUniformLocation(lightShader.getProgram(), "lightColor"), pointLight.color.x, pointLight.color.y, pointLight.color.z);
        
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
    
    if(firstCall) {
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
