#ifndef _TRANS_H
#define _TRANS_H

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLFW_SCROLL_UP 1000
#define GLFW_SCROLL_DOWN 1001

#define ROT_SPEED 0.1f

void set_cdv(glm::vec2 source);

void set_key(int key, bool value);

glm::vec3 calc_translation();

float calc_scale(int &scroll);

glm::vec3 calc_rot_vec();

float calc_rot_angle();

#endif // Trans.h
