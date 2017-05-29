#include "../Includes/Callbacks.h"

namespace Callbacks {

bool wireframe = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
                break;
            case GLFW_KEY_SPACE:
                wireframe = !wireframe;
                glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
                break;
        }
    }
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) { }

}