#pragma once

#include <GLFW/glfw3.h>

namespace Callbacks {
    
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
}