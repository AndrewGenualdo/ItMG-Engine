//
// Created by cobble on 10/12/2024.
//

#include "cube.hpp"

cobb::Cube::Cube(const std::string &path) {
    m_path = path.c_str();
    m_filterMode = GL_NEAREST;
    load();
}

cobb::Cube::Cube(const std::string &path, int filterMode) {
    m_path = path.c_str();
    m_filterMode = filterMode;
    load();
}

cobb::Cube::~Cube() {
    //glDeleteVertexArrays(1, getVAO());
    //glDeleteBuffers(1, getVBO());
}

void cobb::Cube::load() {
    glBindVertexArray(*getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CUBE), VERTICES_CUBE, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //uv coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMode);

    int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(m_path, &m_width, &m_height, &nrChannels, 0);
    int colorType = nrChannels == 3 ? GL_RGB : GL_RGBA;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, colorType, m_width, m_height, 0, colorType, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture: " << m_path << std::endl;
    }

    stbi_image_free(data);
}

void cobb::Cube::bind() {
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void cobb::Cube::draw() {
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

unsigned int cobb::Cube::getId() const {
    return m_id;
}

unsigned int *cobb::Cube::getVAO() {
    if (CUBEMAP_VAO == -1) {
        glGenVertexArrays(1, &CUBEMAP_VAO);
    }

    return &CUBEMAP_VAO;
}

unsigned int *cobb::Cube::getVBO() {
    if (CUBEMAP_VBO == -1) {
        glGenBuffers(1, &CUBEMAP_VBO);
    }

    return &CUBEMAP_VBO;
}