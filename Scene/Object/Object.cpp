#include "Object.h"
#include <iostream>

Object::Object(Shape shape, GLuint VAO) {
    vertices = gen_cube(&(this->dots));

    glGenBuffers(1, &(this->VBO));
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, dots * 8 * sizeof(GLfloat), this->vertices, GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    // Normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *) (6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &(this->diffuseMap));
    glGenTextures(1, &(this->specularMap));

    int width, height;
    unsigned char* image;

    // Diffuse map
    image = SOIL_load_image("Textures/Container/diffuse.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    // Specular map
    image = SOIL_load_image("Textures/Container/specular.png", &width, &height, 0, SOIL_LOAD_RGB);
    glBindTexture(GL_TEXTURE_2D, this->specularMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Object::setModelMatrix(const glm::mat4 &model) {
    this->model = model;
}

const glm::mat4 &Object::getModelMatrix() {
    return this->model;
}

void Object::render() {
    // Bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

    // Bind specular map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->specularMap);

    glDrawArrays(GL_TRIANGLES, 0, dots);
}

Object::~Object() {
    delete[] vertices;
}
