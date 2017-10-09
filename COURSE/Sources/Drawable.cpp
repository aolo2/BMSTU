#define TINYOBJLOADER_IMPLEMENTATION
#include "../Includes/Drawable.h"
#include "../Includes/tiny_obj_loader.h"

#include <glm/glm.hpp>

namespace utils {

void drawable::load_obj(const std::string& path)
{
    tinyobj::attrib_t attrib;

    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;

    bool status = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, path.c_str());

    if (!err.empty()) {
        std::cerr << err << std::endl;
    }

    if (!status) {
        std::exit(1);
    }

    for (std::size_t s = 0; s < shapes.size(); s++) {
        std::size_t index_offset = 0;

        /* Vertices */
        for (std::size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[s].mesh.num_face_vertices[f];
            std::vector<glm::vec3> face_vertices;

            for (std::size_t v = 0; v < fv; v++) {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * idx.vertex_index + 2]);

                normals.push_back(attrib.normals[3 * idx.normal_index + 0]);
                normals.push_back(attrib.normals[3 * idx.normal_index + 1]);
                normals.push_back(attrib.normals[3 * idx.normal_index + 2]);
            }

            index_offset += fv;

            /* Materials */
            int current_material_id = shapes[s].mesh.material_ids[f];

            if (current_material_id < 0) {
                continue;
            }

            for (size_t i = 0; i < 3; i++) {
                colors.push_back(materials[current_material_id].diffuse[0]);
                colors.push_back(materials[current_material_id].diffuse[1]);
                colors.push_back(materials[current_material_id].diffuse[2]);
            }
        }
    }

    std::cout << vertices.size() << " " << colors.size() << " " << normals.size() << std::endl;
}

drawable::drawable(const std::string& mesh_path)
{
    load_obj(mesh_path);

    dimension = 3;
    drawmode = GL_TRIANGLES;

    set_buffers();
}

void drawable::render() const
{
    glBindVertexArray(VAO);
    glDrawArrays(drawmode, 0, vertices.size() / dimension);
    glBindVertexArray(0);
}

void drawable::set_buffers()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, (vertices.size() + normals.size() + colors.size()) * sizeof(GLfloat), 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(GLfloat), vertices.data());
    glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), normals.size() * sizeof(GLfloat), normals.data());
    glBufferSubData(GL_ARRAY_BUFFER, (vertices.size() + normals.size()) * sizeof(GLfloat), colors.size() * sizeof(GLfloat), colors.data());

    glVertexAttribPointer(0, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(GLfloat), (GLvoid*)(vertices.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, dimension, GL_FLOAT, GL_FALSE, dimension * sizeof(GLfloat), (GLvoid*)(vertices.size() + normals.size() * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

drawable::~drawable()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}
}