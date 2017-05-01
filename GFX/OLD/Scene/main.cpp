#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Object/Object.h"

#include <GLFW/glfw3.h>

#include <vector>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

bool keys[1024], cull = false, wireframe = false;
Camera camera;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void process_keyboard(GLFWwindow *window, glm::vec3 &lightPos);

GLuint loadCubemap(std::vector<const GLchar *> faces);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test Scene", glfwGetPrimaryMonitor(), NULL);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader phong("GLSL/phong.vert", "GLSL/phong.frag");
    Shader light("GLSL/light.vert", "GLSL/light.frag");
    Shader skybox("GLSL/skybox.vert", "GLSL/skybox.frag");

    std::vector<const GLchar*> faces;
    faces.push_back("Textures/Skybox/right.png");
    faces.push_back("Textures/Skybox/left.png");
    faces.push_back("Textures/Skybox/top.png");
    faces.push_back("Textures/Skybox/bottom.png");
    faces.push_back("Textures/Skybox/back.png");
    faces.push_back("Textures/Skybox/front.png");
    GLuint cubemapTexture = loadCubemap(faces);

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *) 0);
    glBindVertexArray(0);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    Object cube(Shape::CUBE, VAO);


    std::vector<Object *> objects;
    float r = 2.0f;
    for (int i = 0; i < 10; i++) {
        float phi = 360.0f / 10 * i;
        objects.push_back(new Object(Shape::CUBE, VAO));
        objects[i]->setModelMatrix(glm::translate(glm::mat4(), glm::vec3(r * cos(phi), (float) i * 0.3f, r * sin(phi))));
    }

    glm::mat4 view, proj;
    glm::vec3 lightPos = glm::vec3(0.0f, 2.0f, 3.0f);

    proj = glm::perspective(45.0f, (GLfloat) WINDOW_WIDTH / (GLfloat) WINDOW_HEIGHT, 0.1f, 1000.0f);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        process_keyboard(window, lightPos);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cull ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
        wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        lightPos.z = 3 * cosf(glfwGetTime() / 3);

        phong.useProgram();

        view = camera.viewMatrix();

        phong.setUniform("view", view);
        phong.setUniform("proj", proj);
        phong.setUniform("lightPos", lightPos);
        phong.setUniform("camPos", camera.getPosition());

        phong.setUniform("diffuseMap", 0, false);
        phong.setUniform("specularMap", 1, false);

        glBindVertexArray(VAO);
        for (int i = 0; i < objects.size(); i++) {
            phong.setUniform("model", objects[i]->getModelMatrix());
            objects[i]->render();
        }

        light.useProgram();

        cube.setModelMatrix(glm::translate(glm::mat4(), lightPos));
        cube.setModelMatrix(glm::scale(cube.getModelMatrix(), glm::vec3(0.2f)));

        light.setUniform("model", cube.getModelMatrix());
        light.setUniform("view", view);
        light.setUniform("proj", proj);

        cube.render();

        glBindVertexArray(0);

        glDepthMask(GL_FALSE);
        skybox.useProgram();
        view = glm::mat4(glm::mat3(camera.viewMatrix()));


        skybox.setUniform("proj", proj);
        skybox.setUniform("view", view);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        skybox.setUniform("skybox", 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glDepthMask(GL_TRUE);

        glfwSwapBuffers(window);
    }

    for (int i = 0; i < objects.size();  i++) {
        delete objects[i];
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

GLuint loadCubemap(std::vector<const GLchar*> faces) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for(GLuint i = 0; i < faces.size(); i++) {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

bool firstCall = true;
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

    camera.processCursor(xoffset, yoffset, true);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        if (key == GLFW_KEY_ENTER) cull = !cull;
        else if (key == GLFW_KEY_SPACE) wireframe = !wireframe;
    }

    if (action == GLFW_RELEASE) { keys[key] = false; }
}

void process_keyboard(GLFWwindow *window, glm::vec3 &lightPos) {
    if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);
    if (keys[GLFW_KEY_W]) { camera.processMovement(GLFW_KEY_W); }
    if (keys[GLFW_KEY_A]) { camera.processMovement(GLFW_KEY_A); }
    if (keys[GLFW_KEY_S]) { camera.processMovement(GLFW_KEY_S); }
    if (keys[GLFW_KEY_D]) { camera.processMovement(GLFW_KEY_D); }
}
