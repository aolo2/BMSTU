#include "Shader/Shader.h"
#include "Camera/Camera.h"
#include "Object/Object.h"

#include "STB/stb_image.h"

#include <GLFW/glfw3.h>

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

bool keys[1024], wireframe;
Camera camera;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void do_keyboard(GLFWwindow *window);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "PBR Demo", nullptr, nullptr);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    // glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

    Shader eqtocube("GLSL/cubemap.vert", "GLSL/eqtocube.frag");
    
    Shader irradiance("GLSL/cubemap.vert", "GLSL/irradiance.frag");    
    Shader prefilter("GLSL/cubemap.vert", "GLSL/prefilter.frag");
    Shader LUTgen("GLSL/quad.vert", "GLSL/LUTgen.frag");
    
    Shader PBR("GLSL/pbr.vert", "GLSL/pbr.frag");
    
    Shader skybox("GLSL/skybox.vert", "GLSL/skybox.frag");
    Shader quadShader("GLSL/quad.vert", "GLSL/quad.frag");

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (GLfloat) WINDOW_WIDTH / (GLfloat) WINDOW_HEIGHT, 0.1f, 100.0f);
    glm::vec3 lightPos(0.0f, 0.0f, 2.0f);

    quadShader.useProgram();
    quadShader.setUniform1i("lut", 0);

    skybox.useProgram();
    skybox.setUniform("proj", proj);
    skybox.setUniform1i("equirectangularMap", 0);

    PBR.useProgram();
    PBR.setUniform("proj", proj);
    PBR.setUniform1i("irradianceMap", 0);
    PBR.setUniform1i("prefilterMap", 1);
    PBR.setUniform1i("BRDFlut", 2);

    GLuint VAO, cubeVAO, quadVAO;
    glGenVertexArrays(1, &VAO);
    glGenVertex(1, &cubeVAO);
    glGenVertexArrays(1, &quadVAO);

    Object sphere(VAO, Shape::SPHERE);
    Object cube(cubeVAO, Shape::CUBE), quad(quadVAO, Shape::QUAD);

    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


    // Load HDR texture with STB

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    float *data = stbi_loadf("Textures/Skybox/Uffizi/Uffizi.hdr", &width, &height, &nrComponents, 0);
    GLuint hdrTexture;

    if (data) {
        glGenTextures(1, &hdrTexture);
        glBindTexture(GL_TEXTURE_2D, hdrTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_FLOAT, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Failed to load HDR image." << std::endl;
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    // ==================================================



    // Environment cubemap generation

    GLuint envCubemap;
    glGenTextures(1, &envCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 3);

    for (GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    glm::mat4 captureViews[] = {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
    };


    eqtocube.useProgram();
    eqtocube.setUniform1i("equirectangularMap", 0);
    eqtocube.setUniform("proj", captureProjection);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);

    glViewport(0, 0, 1024, 1024);
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

    for (GLuint i = 0; i < 6; i++) {
        eqtocube.setUniform("view", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(cubeVAO);
        cube.render();
    }


    // ==================================================



    // Irradiance map generation

    GLuint irradianceMap;
    glGenTextures(1, &irradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 3);

    for (GLuint i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 256, 256, 0, GL_RGB, GL_FLOAT, nullptr);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 256, 256);

    irradiance.useProgram();
    irradiance.setUniform1i("environmentMap", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    irradiance.setUniform("proj", captureProjection);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    for (unsigned int i = 0; i < 6; i++) {
        float roughness = 0.0f;
        for (int j = 0; j < 4; j++, roughness += 0.25f) {
            glViewport(0, 0, 256 / pow(2, j), 256 / pow(2, j));
            irradiance.setUniform("roughness", roughness);
            irradiance.setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap, j);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(cubeVAO);
            cube.render();
        }
    }

    // ==================================================




    // Prefilter environment map (first term of split-sum)
    
    GLuint prefilteredMap;
    glGenTextures(1, &prefilteredMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, prefilteredMap);
    
    for (unsigned int i = 0; i < 6; i++) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 256, 256, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    
    prefilter.useProgram();
    prefilter.setUniform1i("environmentMap", 0);
    prefilter.setUniform("proj", captureProjection);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);
    
    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    
    for (unsigned int i = 0; i < 6; i++) {
        float roughness = 0.0f;
        for (int j = 0; j < 4; j++, roughness += 0.25f) {
            glViewport(0, 0, 256 / pow(2, j), 256 / pow(2, j));
            prefilter.setUniform("roughness", roughness);
            prefilter.setUniform("view", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilteredMap, j);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glBindVertexArray(cubeVAO);
            cube.render();
        }
    }
    
    // ==================================================



    // 2D LUT BRDF generation (second term of split-sum)

    GLuint brdfLUT;
    glGenTextures(1, &brdfLUT);
    glBindTexture(GL_TEXTURE_2D, brdfLUT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 256, 256, 0, GL_RG, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 256, 256);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT, 0);

    glViewport(0, 0, 256, 256);
    LUTgen.useProgram();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindVertexArray(quadVAO);
    quad.render();
    glBindVertexArray(0);
    // ==================================================



    glm::mat4 model, view;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    float now = 0.0f, start = glfwGetTime(), then = start;
    int frames;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        do_keyboard(window);
        wireframe ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


// Bright stripe on LUT near NoV = 0, is it ok?
/*
        glViewport(0, 0, 256, 256);
        
        quadShader.useProgram();
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);
        
        glBindVertexArray(quadVAO);
        quad.render();
        glBindVertexArray(0);
        
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
*/



        PBR.useProgram();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceMap);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, prefilteredMap);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brdfLUT);

        view = camera.viewMatrix();

        PBR.setUniform("view", view);
        PBR.setUniform("camPos", camera.getPosition());

        glBindVertexArray(VAO);
        for (int i = 0; i < 7; i++) {
            PBR.setUniform("roughness", (float) i / 6.0f);
            for (float m = 0; m < 7; m++) {
                model = glm::scale(glm::mat4(), glm::vec3(1.0f));
                model = glm::translate(model, glm::vec3(1.5f * i, m * 1.5f, 0.0f));

                PBR.setUniform("model", model);
                PBR.setUniform("metallic", m / 6.0f);

                sphere.render();
            }
        }


        skybox.useProgram();
        skybox.setUniform("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap);

        glBindVertexArray(cubeVAO);
        cube.render();

        glBindVertexArray(0);
        glfwSwapBuffers(window);
/*
        frames++;
        now = glfwGetTime();
        printf("%.3f\n", (now - then) * 1000);
        then = now;

        if (now - start >= 10.0f) {
            std::cout << "Total frames: " << frames << std::endl;
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

*/
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &cubeVAO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}


double lastX, lastY;
bool firstCall = true;
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstCall) {
        lastX = xpos;
        lastY = ypos;
        firstCall = false;
    }

    camera.processCursor(xpos - lastX, lastY - ypos, true);
    lastX = xpos;
    lastY = ypos;
}

void do_keyboard(GLFWwindow *window) {
    if (keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(window, GL_TRUE);

    if (keys[GLFW_KEY_W]) camera.processMovement(GLFW_KEY_W);
    if (keys[GLFW_KEY_A]) camera.processMovement(GLFW_KEY_A);
    if (keys[GLFW_KEY_S]) camera.processMovement(GLFW_KEY_S);
    if (keys[GLFW_KEY_D]) camera.processMovement(GLFW_KEY_D);
}

// ===== Callbacks ======
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        if (key == GLFW_KEY_SPACE) wireframe = !wireframe;
    }

    if (action == GLFW_RELEASE) { keys[key] = false; }
}
