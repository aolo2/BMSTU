#include "Scene.h"

const std::string root = "Scene";

void save_scene(const Scene &scene) {
    std::ofstream f;
    
    std::cout << "# Saving scene" << std::endl;
    
    f.open((root + separator()) + "Scene1.scn");
    
    f << "lab_6\n\n";
    f << "start_pos = " << scene.start_pos.x << " " << scene.start_pos.y << " " << scene.start_pos.z << std::endl;
    f << "start_dir = " << scene.start_dir.x << " " << scene.start_dir.y << " " << scene.start_dir.z << std::endl;
    f << "light_pos = " << scene.light_pos.x << " " << scene.light_pos.y << " " << scene.light_pos.z << std::endl;
    f << std::endl;
    f << "textured = " << scene.textured << std::endl;
    f << "paused = " << scene.paused << std::endl;
    f << "diffuse = " << scene.diffuse << std::endl;
    f << "time_elapsed = " << scene.time_elapsed << std::endl;
    f << "speed = " << scene.speed << std::endl;
    f << "light_speed = " << scene.light_speed << std::endl;
    
    f.close();
    
    std::cout << "# Done" << std::endl;
}

float parse_float(const std::string &line) {
    return std::stof(line.substr(line.find("=") + 2), nullptr);
}

glm::vec3 parse_vec3(const std::string &line) {
    std::string values = line.substr(line.find("=") + 2);
    std::string::size_type sz(0), sz2(0);
    float x, y, z;
    
    x = std::stof(values, &sz);
    y = std::stof(values.substr(sz), &sz2);
    z = std::stof(values.substr(sz + sz2));
    
    return glm::vec3(x, y, z);
}

Scene load_scene(const std::string &path) {
    std::string new_path = (root + separator()) + path;
    std::ifstream f(new_path);
    
    unsigned int line_number(0);
    std::string line;

    // read scene title
    std::getline(f, line);
    std::cout << "# Loading scene \'" << line << "\'" << std::endl;
    ++line_number;
    
    Scene scene;
    
    // start_pos
    while (std::getline(f, line)) {
        std::string var_name(line.substr(0, line.find(" =")));
        
        if (var_name.size() == 0) {
            continue;
        }
        
        std::string values;

        std::cout << "# Parsing \'" << var_name << "\'" << std::endl;

        if (var_name == "start_pos") { scene.start_pos = parse_vec3(line); }
        else if (var_name == "start_dir") { scene.start_dir = parse_vec3(line); }
        else if (var_name == "light_pos") { scene.light_pos = parse_vec3(line); }
        else if (var_name == "paused") { scene.paused = (parse_float(line) != 0); }
        else if (var_name == "textured") { scene.textured = (parse_float(line) != 0); }
        else if (var_name == "diffuse") { scene.diffuse = parse_float(line); }
        else if (var_name == "speed") { scene.speed = parse_float(line); }
        else if (var_name == "time_elapsed") { scene.time_elapsed = parse_float(line); }
        else if (var_name == "light_speed") { scene.light_speed = parse_float(line); }
        else if (var_name.size() > 0) {
            std::cout << "# Unknown variable \'" << var_name << "\' at line " << line_number << std::endl;
        }
        
        ++line_number;
    }
            
    std::cout << "# Done\n" << std::endl;
    
    return scene;
}