#include "Shader.h"
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

bool keys[1000];
const float speed = 0.03f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

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

void process_movement(glm::vec3 &lightPos) {
    if (keys[GLFW_KEY_Q]) { lightPos.z += speed; }
    if (keys[GLFW_KEY_W]) { lightPos.y += speed; }
    if (keys[GLFW_KEY_E]) { lightPos.z -= speed; }
    
    if (keys[GLFW_KEY_A]) { lightPos.x -= speed; }
    if (keys[GLFW_KEY_S]) { lightPos.y -= speed; }
    if (keys[GLFW_KEY_D]) { lightPos.x += speed; }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    #ifdef DBG_MODE
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
    #endif
    
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LEQUAL); // depth-testing interprets a smaller value as "closer"    
    
    glfwSetKeyCallback(window, key_callback);  
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    
    std::vector<glm::vec3> vertices_vector, light_vertices;
    std::vector<int> index_vector, light_index;
    
    gen_sphere(vertices_vector, index_vector, 0.5f, 60, 60);
    gen_sphere(light_vertices, light_index, 0.2f, 2, 2);
    
    GLfloat vertices[vertices_vector.size() * 3];
    for (int i = 0; i < vertices_vector.size(); i++) { 
        vertices[3 * i] = vertices_vector[i].x;
        vertices[3 * i + 1] = vertices_vector[i].y;
        vertices[3 * i + 2] = vertices_vector[i].z;
    }
    
    GLuint indices[index_vector.size()];
    for (int i = 0; i < index_vector.size(); i++) {
        indices[i] = index_vector[i];
    }
    
    GLfloat lightVertices[light_vertices.size() * 3];
    GLuint lightIndices[light_index.size()];
    
    for (int i = 0; i < light_vertices.size(); i++) {
        lightVertices[3 * i] = light_vertices[i].x;
        lightVertices[3 * i + 1] = light_vertices[i].y;
        lightVertices[3 * i + 2] = light_vertices[i].z;
    }
    
    for (int i = 0; i < light_index.size(); i++) {
        lightIndices[i] = light_index[i];
    }
    
    GLuint VBO, VAO, EBO;
    GLuint lightVBO, lightEBO;

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &lightVBO);
    glGenBuffers(1, &lightEBO);

    Shader shaderProgram("default.vert", "default.frag");
    Shader lightShader("light.vert", "light.frag");
    
    glm::vec3 lightPos(0.5f, 0.5f, 0.5f);
    glm::mat4 model, view, proj;

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));
    proj = glm::perspective(45.0f, (GLfloat) WINDOW_WIDTH / (GLfloat) WINDOW_HEIGHT, 0.1f, 100.0f);

    GLint modelLoc, viewLoc, projLoc, camPosLoc;
    GLint lightModelLoc, lightColorLoc, objectColorLoc, lightPosLoc;
    
    glClearColor(0.00f, 0.00f, 0.02f, 1.0f);    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);
        
        /* ===========DRAW HERE============= */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GLfloat time = (GLfloat) glfwGetTime();
        
        
        shaderProgram.useProgram();
        
        camPosLoc = glGetUniformLocation(shaderProgram.getProgram(), "camPos");
        lightPosLoc = glGetUniformLocation(shaderProgram.getProgram(), "lightPos");     
        lightColorLoc  = glGetUniformLocation(shaderProgram.getProgram(), "lightColor");        
        objectColorLoc = glGetUniformLocation(shaderProgram.getProgram(), "objectColor");
        
        modelLoc = glGetUniformLocation(shaderProgram.getProgram(), "model");
        viewLoc = glGetUniformLocation(shaderProgram.getProgram(), "view");
        projLoc = glGetUniformLocation(shaderProgram.getProgram(), "proj");

        model = glm::translate(glm::mat4(), glm::vec3(0.0f, sin(time) / 2, 0.0f));

        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        
        glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.15f);
        glUniform3f(lightColorLoc,  1.0f, 1.0f, 1.0f);
        glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(camPosLoc, 0.0f, 0.0f, 3.0f); // glm::translate * (-1)
    
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
        
    
        glDrawElements(GL_TRIANGLES, index_vector.size(), GL_UNSIGNED_INT, 0);
        
        glBindVertexArray(0);
        
        
        lightShader.useProgram();
        
        modelLoc = glGetUniformLocation(lightShader.getProgram(), "model");
        viewLoc = glGetUniformLocation(lightShader.getProgram(), "view");
        projLoc = glGetUniformLocation(lightShader.getProgram(), "proj");

        model = glm::translate(glm::mat4(), glm::vec3(lightPos));
        model = glm::scale(model, glm::vec3(0.5f));

        process_movement(lightPos);
        
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(lightVertices), lightVertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*) 0);
        glEnableVertexAttribArray(0);
                
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lightIndices), lightIndices, GL_STATIC_DRAW); 
        

        glDrawElements(GL_TRIANGLES, light_index.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        /* ================================= */
                
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

bool lmb_pressed, rmb_pressed;

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
}
