#define GLEW_STATIC

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream> 
#include <vector>
#include <GL/glew.h>

// GLFW (mouse and keyboard events, windows)
#include <GLFW/glfw3.h>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define CURSOR_SIZE 8

struct Point {
    float x, y;
};

glm::mat4 trans, last_trans;
glm::vec3 color;

GLFWimage image;
GLFWcursor* cursor;
unsigned char pixels[CURSOR_SIZE * CURSOR_SIZE * 4];

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

void set_cursor_color(int r, int g, int b, int a) {
    for (int i = 0; i < CURSOR_SIZE * CURSOR_SIZE * 4; i += 4) {
        pixels[i] = r;
        pixels[i + 1] = g;
        pixels[i + 2] = b;
        pixels[i + 3] = a;
    }
}

Point px_to_screen(Point src) {
    Point screen;
    
    screen.x = (src.x - WINDOW_WIDTH / 2) / (WINDOW_WIDTH / 2);
    screen.y = (WINDOW_HEIGHT / 2 - src.y ) / (WINDOW_HEIGHT / 2);
    
    return screen;
}

int make_shaders(const char* vs_src, const char* fs_src, GLuint* shaderProgram) {
    GLuint vs, fs;
    GLint success;
    GLchar infoLog[512];
    
    vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_src, NULL);
    glCompileShader(vs);
    
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 1;
    }
   
    
    fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_src, NULL);
    glCompileShader(fs);
    
    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 1;
    }
    
    glAttachShader(*shaderProgram, fs);
    glAttachShader(*shaderProgram, vs);
    glLinkProgram(*shaderProgram);
    
    glGetProgramiv(*shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(*shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 1;
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);
        
    return 0;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test", nullptr, nullptr);
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
    
    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
  
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    glfwSetKeyCallback(window, key_callback);  
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
    set_cursor_color(0, 0, 0, 255);
    image.width = CURSOR_SIZE;
    image.height = CURSOR_SIZE;
    image.pixels = pixels;
    cursor = glfwCreateCursor(&image, 0, 0);
    glfwSetCursor(window, cursor);
    
    Point s1, s2, s3, s4;
    s1 = {0, 0};
    s2 = px_to_screen({408, 300});
    s3 = px_to_screen({400, 292});
    s4 = px_to_screen({408, 292});
    

    // tmp vertices
    GLfloat vertices[] = {
        s2.x, s2.y, 0.0f,  // Top Right
        s4.x, s4.y, 0.0f,  // Bottom Right
        s3.x, s3.y, 0.0f,  // Bottom Left
        s1.x, s1.y, 0.0f   // Top Left 
    };
    
    
    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,   // First Triangle
        1, 2, 3    // Second Triangle
    };  
    
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

    
    GLuint shaderProgram = glCreateProgram();
    
    std::ifstream t1("default.frag");
    std::string f_src((std::istreambuf_iterator<char>(t1)),
                     std::istreambuf_iterator<char>());

    std::ifstream t2("default.vert");
    std::string v_src((std::istreambuf_iterator<char>(t2)),
                     std::istreambuf_iterator<char>());

                     
    const char* VERTEX_SHADER_SRC = v_src.c_str();
    const char* FRAGMENT_SHADER_SRC = f_src.c_str();
    
    if (make_shaders(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC, &shaderProgram) != 0) {
        std::cerr << "Could not finish shader setup, exiting" << std::endl;
        return 1;
    }
    
    GLuint transformLoc = glGetUniformLocation(shaderProgram, "transform");
    GLuint revertLoc = glGetUniformLocation(shaderProgram, "revert");
    GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");
    
    glClearColor(0.15f, 0.15f, 0.16f, 1.0f);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    bool ab = true;
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        glUniformMatrix4fv(revertLoc, 1, GL_FALSE, glm::value_ptr(last_trans));
        glUniform3fv(colorLoc, 1, glm::value_ptr(color));
    
        if (ab) {
            glClear(GL_COLOR_BUFFER_BIT);
            ab = false;
        }
    
        /* ===========DRAW HERE============= */
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        /* ================================= */
                
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

bool lmb_pressed, rmb_pressed;
Point scrspccrd;

double mx, my;


void cursor_pos_callback(GLFWwindow* window, double l_xpos, double l_ypos) {
    scrspccrd = px_to_screen({l_xpos, l_ypos});
    // printf("%.2f %.2f\n", scrspccrd.x, scrspccrd.y);
    
    mx = scrspccrd.x, my = scrspccrd.y;
    
    if (lmb_pressed) {
        last_trans = glm::inverse(trans);
        trans = glm::translate(trans, glm::vec3(mx, my, 0.0));
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) { 
            lmb_pressed = true; 
            last_trans = glm::inverse(trans);
            trans = glm::translate(trans, glm::vec3(mx, my, 0.0));
        }
        
        if (button == GLFW_MOUSE_BUTTON_RIGHT) { rmb_pressed = true; }
    } else if (action == GLFW_RELEASE) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            lmb_pressed = false; 
            // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        if (button == GLFW_MOUSE_BUTTON_RIGHT) { rmb_pressed = false; }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                set_cursor_color(255, 0, 0, 255);
                color = {255, 0, 0};
                break;
            case GLFW_KEY_2:
                set_cursor_color(0, 255, 0, 255);
                color = {0, 255, 0};
                break;
            case GLFW_KEY_3:
                set_cursor_color(0, 0, 255, 255);
                color = {0, 0, 255};
                break;
            case GLFW_KEY_0:
                set_cursor_color(255, 255, 255, 255);
                color = {255, 255, 255};
                break;
            case GLFW_KEY_SPACE:
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                break;
            default:
                std::cout << key << std::endl;
        }
        
        image.pixels = pixels;
        cursor = glfwCreateCursor(&image, 0, 0);
        glfwSetCursor(window, cursor);
    }
}    
