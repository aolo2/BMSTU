#include "../Includes/Callbacks.h"

namespace callback {

bool first_call = true, wireframe = false;
double last_x = 0.0, last_y = 0.0;

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    double x_offset, y_offset;

    if (!first_call) {
        x_offset = xpos - last_x;
        y_offset = ypos - last_y;
    } else {
        x_offset = xpos;
        y_offset = ypos;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_SPACE:
            wireframe = !wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        default:
            break;
        }
    } else if (action == GLFW_RELEASE) {
        // ... 
    }
}
}