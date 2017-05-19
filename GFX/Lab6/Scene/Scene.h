#pragma once

// GLM (maths)
#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <streambuf>
#include <iostream>

struct Scene {
    glm::vec3 start_pos;
    glm::vec3 start_dir;
    glm::vec3 light_pos;

    float light_speed;
    float time_elapsed;
    float diffuse;
    float speed;
    
    bool textured;
    bool paused;
};

inline char separator() {
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

Scene load_scene(const std::string &path);
void save_scene(const Scene &scene);
