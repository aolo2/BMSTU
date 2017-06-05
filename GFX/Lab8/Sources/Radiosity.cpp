#include "../Includes/Radiosity.h"

namespace VPL {
    
    /*
    1. Locate the light object (color > 1.0f hack)
    2. Sample [samples] points from light (?surface?)
    3. Shoot a ray in random direction (from hemishpere) from each point
    4. Find first intersection with a scene object
    5. Intersection = new VPL position, color of this point = VPL color
    6. recursive call?
    */
    
    
    /*
    // std::vector<glm::vec3> place_vpls(const std::vector<Utils::Object> &scene, 
    unsigned int samples, unsigned int bounces) {
        
        Utils::Object light(Utils::Shape::CUBE);
        
        // 1. Locate the light object
        for (const auto &object : scene) {
            if (object.gcolors()[0] > 1.0f) {
                light = object;
            }
        }
        
        // sampling
        // see http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
    }
    */
}
