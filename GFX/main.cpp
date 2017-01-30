#define GLEW_STATIC

// iostream and GLEW included in Shader.h
#include <iostream> 
#include <GL/glew.h>

// GLFW (mouse and keyboard events, windows)
#include <GLFW/glfw3.h>

// GLM (maths)
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* VERTEX_SHADER_SRC = "#version 430\n"
                                "in vec3 inPos;"
                                "void main() {"
                                "  gl_Position = vec4(inPos, 1.0);"
                                "}";
                                
const char* FRAGMENT_SHADER_SRC = "#version 430\n"
                                  "out vec4 outColor;"
                                  "void main() {"
                                  "  outColor = vec4(1.0, 0.5, 0.0, 1.0);"
                                  "}";                                

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
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

    // tmp vertices
    GLfloat vertices[] = {
        0.5f, -0.5f, 0.0f,
       -0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
    };
    
    GLuint VBO = 0, VAO = 0;
    
    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), vertices, GL_STATIC_DRAW);
    
    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    
    GLuint shaderProgram = glCreateProgram();
    if (make_shaders(VERTEX_SHADER_SRC, FRAGMENT_SHADER_SRC, &shaderProgram) != 0) {
        std::cerr << "Could not finish shader setup, exiting" << std::endl;
        return 1;
    }
    
    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        /* ===========DRAW HERE============= */
        glClearColor(0.15f, 0.15f, 0.18f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        /* ================================= */
                
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_1:
                //red 
                break;
            case GLFW_KEY_2:
                //green
                break;
            case GLFW_KEY_3:
                //blue
                break;
            case GLFW_KEY_0:
                //white
                break;
            case GLFW_KEY_SPACE:
                //clear
                break;
            default:
                std::cout << key << std::endl;
        }
    }
}    
