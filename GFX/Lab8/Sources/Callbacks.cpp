#include "../Includes/Callbacks.h"

namespace Callbacks {

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE);
        }
    }
}

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) { }

}