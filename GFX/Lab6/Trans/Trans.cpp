#include "Trans.h"

bool keys[1024];
glm::vec2 cdv;

void set_cdv(glm::vec2 source) {
    cdv = source;
}

void set_key(int key, bool value) {
    keys[key] = value;
}

glm::vec3 calc_translation() {
    glm::vec3 new_pos;

    if (keys[GLFW_KEY_W]) new_pos += glm::vec3(0.0f, 0.01f, 0.0f);
    if (keys[GLFW_KEY_S]) new_pos -= glm::vec3(0.0f, 0.01f, 0.0f);

    if (keys[GLFW_KEY_D]) new_pos += glm::vec3(0.01f, 0.0f, 0.0f);
    if (keys[GLFW_KEY_A]) new_pos -= glm::vec3(0.01f, 0.0f, 0.0f);

    if (keys[GLFW_KEY_Q]) new_pos += glm::vec3(0.0f, 0.0f, 0.01f);
    if (keys[GLFW_KEY_E]) new_pos -= glm::vec3(0.0f, 0.0f, 0.01f);

    return new_pos;
}

float calc_scale(int &scroll) {

    if (scroll == 0) {
        keys[GLFW_SCROLL_UP] = false;
        keys[GLFW_SCROLL_DOWN] = false;
    } else if (scroll > 0) {
        keys[GLFW_SCROLL_UP] = true;
        keys[GLFW_SCROLL_DOWN] = false;
    } else {
        keys[GLFW_SCROLL_UP] = false;
        keys[GLFW_SCROLL_DOWN] = true;
    }

    scroll = 0;

    if (keys[GLFW_SCROLL_UP]) return 1.1f;
    if (keys[GLFW_SCROLL_DOWN]) return 0.9f;

    return 1.0f;
}

glm::vec3 calc_rot_vec() {
    return glm::vec3(cdv.y, -1 * cdv.x, 0.0f);
}

float calc_rot_angle() {
    return glm::length(cdv);
}
