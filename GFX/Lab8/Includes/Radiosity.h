#pragma once

#include "Object.h"

namespace VPL {
    std::vector<glm::vec3> place_vpls(const std::vector<Utils::Object> &scene, 
    unsigned int samples, unsigned int bounces);
}