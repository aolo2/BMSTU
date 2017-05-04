#include "Shader/Shader.h"
#include "Shapes/Shapes.h"
#include "Trans/Trans.h"

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;

#include <queue>

#define MAX_QUEUE 5

int scroll, slices = 20;
float absscale = 1.0f;
bool wireframe, lmb, localkeys[1000];

GLfloat *surface;
GLuint VAO, VBO;
glm::mat4 ortho = glm::ortho((float) WINDOW_WIDTH / (float) WINDOW_HEIGHT * -1.0f,
    (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT, -1.0f, 1.0f, -1.0f, 1.0f);
std::queue<glm::vec2> cursor_positions;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

glm::mat4 horizontalIsometry();
glm::vec2 pixel_to_window(glm::vec2 src);
void clear(std::queue<glm::vec2> &q);
void cursor_vec(double xpos, double ypos);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Lab 6", nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    // glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // glLineWidth(2.0f);

    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    
    
    GLuint lightVAO, lightVBO;
    
    GLfloat *light = gen_cube();
    surface = gen_surface(slices);




    // surface
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (slices + 1) * 10 * 12 * sizeof(GLfloat) + (slices + 1) * 12 * sizeof(GLfloat), surface, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // =============
 
    
    
    // light cube
    
    glGenVertexArrays(1, &lightVAO);
    glGenBuffers(1, &lightVBO);    
    
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), light, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // =============

    Shader defaultShader("GLSL/default.vert", "GLSL/default.frag");
    Shader allWhite("GLSL/allwhite.vert", "GLSL/allwhite.frag");

    glm::mat4 view, model, proj, lightModel;
    glm::mat3 inv_model;

    glm::mat4 customProjMatrix = horizontalIsometry();

    glm::vec3 lightPos(0.5f), position, rotate_vec(0.0f, 1.0f, 0.0f); // fix Phong shader (add uniform)
    float scale, rotate_angle = 1.0f;
    double cursor_x, cursor_y;

    glClearColor(0.1f, 0.1f, 0.11f, 1.0f);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        proj = ortho * customProjMatrix;
        
        allWhite.useProgram();
        
        if (localkeys[GLFW_KEY_UP]) lightPos += glm::vec3(0.0f, 0.1f, 0.0f);
        if (localkeys[GLFW_KEY_LEFT]) lightPos -= glm::vec3(0.1f, 0.0f, 0.0f);
        if (localkeys[GLFW_KEY_RIGHT]) lightPos += glm::vec3(0.1f, 0.0f, 0.0f);
        if (localkeys[GLFW_KEY_DOWN]) lightPos -= glm::vec3(0.0f, 0.1f, 0.0f);
        
        lightModel = glm::translate(glm::mat4(), lightPos);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        
        allWhite.setUniform("proj", proj);
        allWhite.setUniform("model", lightModel);
        
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
                
        defaultShader.useProgram();

        if (lmb) {
            glfwGetCursorPos(window, &cursor_x, &cursor_y);
            cursor_vec(cursor_x, cursor_y);
        } else {
            clear(cursor_positions);
        }

        inv_model = glm::inverse(glm::mat3(model));

        rotate_vec = calc_rot_vec(); // rotation axis (absolute)
        rotate_vec = inv_model * rotate_vec; // rotation axis (relative)

        position = calc_translation(); // positon (absolute)
        position = inv_model * position; // position (relative)

        scale = calc_scale(scroll); // scale
        rotate_angle = calc_rot_angle(); // rotation angle

        absscale *= scale;

        model = glm::translate(model, position);
        if (rotate_angle > 0) model = glm::rotate(model, rotate_angle, rotate_vec);
        model = glm::scale(model, glm::vec3(scale));

        wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) :
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        defaultShader.setUniform("lightPos", lightPos);
        defaultShader.setUniform("model", model);
        defaultShader.setUniform("proj", proj);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (slices + 1));
        glDrawArrays(GL_TRIANGLE_STRIP, (slices + 1), (slices + 1) * 20);
        glDrawArrays(GL_TRIANGLE_FAN, (slices + 1) * 21, (slices + 1));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    delete[] light;
    delete[] surface;

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

glm::mat4 horizontalIsometry() {
    glm::mat4 T2;

    T2[2][0] = sqrt(2) / -2.0f;
    T2[2][1] = sqrt(2) / -2.0f;

    return T2;
}

glm::vec2 pixel_to_window(double x, double y) {
    glm::vec2 screen;

    screen.x = (x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
    screen.y = (WINDOW_HEIGHT / 2 - y ) / (WINDOW_HEIGHT / 2);

    return screen;
}

// ===== Callbacks ======
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        set_key(key, true);
        localkeys[key] = true;

        if (key == GLFW_KEY_ESCAPE) glfwSetWindowShouldClose(window, GL_TRUE);
        if (key == GLFW_KEY_SPACE) wireframe = !wireframe;

        if (key == GLFW_KEY_KP_ADD || key == GLFW_KEY_KP_SUBTRACT) {
            
            delete[] surface;

            if (key == GLFW_KEY_KP_ADD) {
                slices++;
            } else if (slices > 3) {
                slices--;
            }

            surface = gen_surface(slices);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, (slices + 1) * 10 * 12 * sizeof(GLfloat) + (slices + 1) * 12 * sizeof(GLfloat), surface, GL_STATIC_DRAW);
          
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
            glEnableVertexAttribArray(0);
          
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
            glEnableVertexAttribArray(1);
          
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }
    }

    if (action == GLFW_RELEASE) {
        set_key(key, false);
        localkeys[key] = false;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    float ratio = (float) width / (float) height;
    ortho = glm::ortho(-ratio, ratio, -1.0f, 1.0f, -1.0f, 1.0f);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        lmb = (action == GLFW_PRESS);
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    scroll = (yoffset > 0) ? 1 : -1;
}

// ===== Cursor calculations =====
void cursor_vec(double xpos, double ypos) {
    cursor_positions.push(pixel_to_window(xpos, ypos));
    while (cursor_positions.size() > MAX_QUEUE) { cursor_positions.pop(); }

    if (cursor_positions.size() > 1) {
        glm::vec2 cpf = cursor_positions.front();
        glm::vec2 cpb = cursor_positions.back();

        glm::vec2 dir = cpf - cpb;
        dir /= absscale * 5.0f;

        set_cdv(dir);
    }
}

void clear(std::queue<glm::vec2> &q) {
    std::queue<glm::vec2> empty;
    std::swap(q, empty);
}
