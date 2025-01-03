//
// Created by cobble on 9/25/2024.
//

#include "texture2d.h"

mat4 cobb::Texture2d::orthoProj = mat4(1);
cobb::Window *cobb::Texture2d::window = nullptr;

cobb::Texture2d::Texture2d(const std::string &path) {
    float vertices[8] = {1, 1, 1, -1, -1, -1, -1, 1};
    loadVertices(vertices);
    m_path = path.c_str();
    m_filterMode = GL_NEAREST;
    m_wrapMode = GL_REPEAT;
    load();
}

cobb::Texture2d::Texture2d(const std::string &path, float positions[8]) {
    loadVertices(positions);
    m_path = path.c_str();
    m_filterMode = GL_NEAREST;
    m_wrapMode = GL_REPEAT;
    load();
}

cobb::Texture2d::Texture2d(const std::string &path, int filterMode, int wrapMode, float positions[8]) {
    loadVertices(positions);
    m_path = path.c_str();
    m_filterMode = filterMode;
    m_wrapMode = wrapMode;
    load();
}

void cobb::Texture2d::load() {
    glBindVertexArray(*getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *getEBO());
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void *>(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapMode);
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
    std::cout << "Successfully loaded texture: " << m_path << std::endl;
}

cobb::Texture2d::~Texture2d() {
    //glDeleteVertexArrays(1, getVAO());
    //glDeleteBuffers(1, getVBO());
    //glDeleteBuffers(1, getEBO());
}

void cobb::Texture2d::bind() const {
    glBindTexture(GL_TEXTURE_2D, m_id);
}

float cobb::Texture2d::getWidth() const {
    return m_width;
}

float cobb::Texture2d::getHeight() const {
    return m_height;
}


void cobb::Texture2d::loadVertices(const float *positions) {
    // corners (top right, bottom right, bottom left, top left) (4)
    // positions (xyz) (3)
    // color (rgba) (4)
    // texture coords (xy) ((0,0) is bottom left) (2)
    //4 * (3 + 4 + 2) = 36 data points
    vertices[0] = positions[0]; //screen top right x
    vertices[1] = positions[1]; //screen top right y
    vertices[2] = 0.0f; //screen top right z
    vertices[3] = 1.0f; //top right r
    vertices[4] = 1.0f; //top right g
    vertices[5] = 1.0f; //top right b
    vertices[6] = 1.0f; //top right a
    vertices[7] = 1.0f; //texture top right x
    vertices[8] = 1.0f; //texture top right y
    vertices[9] = positions[2]; //screen bottom right x
    vertices[10] = positions[3]; //screen bottom right y
    vertices[11] = 0.0f; //screen bottom right z
    vertices[12] = 1.0f; //bottom right r
    vertices[13] = 1.0f; //bottom right g
    vertices[14] = 1.0f; //bottom right b
    vertices[15] = 1.0f; //bottom right a
    vertices[16] = 1.0f; //texture bottom right x
    vertices[17] = 0.0f; //texture bottom right y
    vertices[18] = positions[4]; //screen bottom left x
    vertices[19] = positions[5]; //screen bottom left y
    vertices[20] = 0.0f; //screen bottom left z
    vertices[21] = 1.0f; //bottom left r
    vertices[22] = 1.0f; //bottom left g
    vertices[23] = 1.0f; //bottom left b
    vertices[24] = 1.0f; //bottom left a
    vertices[25] = 0.0f; //texture bottom left x
    vertices[26] = 0.0f; //texture bottom left y
    vertices[27] = positions[6]; //screen top left x
    vertices[28] = positions[7]; //screen top left y
    vertices[29] = 0.0f; //screen top left z
    vertices[30] = 1.0f; //top left r
    vertices[31] = 1.0f; //top left g
    vertices[32] = 1.0f; //top left b
    vertices[33] = 1.0f; //top left a
    vertices[34] = 0.0f; //texture top left x
    vertices[35] = 1.0f; //texture top left y;
}

unsigned int cobb::Texture2d::getId() const {
    return m_id;
}

unsigned int *cobb::Texture2d::getVAO() {
    if (VAO == -1) {
        glGenVertexArrays(1, &VAO);
    }

    return &VAO;
}

unsigned int *cobb::Texture2d::getEBO() {
    if (EBO == -1) {
        glGenBuffers(1, &EBO);
    }

    return &EBO;
}

unsigned int *cobb::Texture2d::getVBO() {
    if (VBO == -1) {
        glGenBuffers(1, &VBO);
    }

    return &VBO;
}

void cobb::Texture2d::loadShader() {
    if (texture2dShader == nullptr) {
        texture2dShader = new Shader("assets/texture2d");
        texture2dShader->use();
        texture2dShader->setVec4("color", vec4(1));
        glBindVertexArray(*getVAO());
        std::cout << "Loaded texture2d shader!" << std::endl;
    }
}

void cobb::Texture2d::setOrtho(mat4 ortho) {
    orthoProj = ortho;
}

void cobb::Texture2d::draw(float x, float y, float scale, bool shouldBind) const {
    loadShader();
    glDisable(GL_DEPTH_TEST);
    if (shouldBind) bind();
    texture2dShader->use();
    glBindVertexArray(*getVAO());
    texture2dShader->setMat4("proj", orthoProj);
    texture2dShader->setMat4("model", Object::translate(x + getWidth() * scale * 0.5f, y + getHeight() * scale, 0) * Object::scale(getWidth() * scale, getHeight() * scale, 1));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void cobb::Texture2d::drawRaw(float x, float y, float width, float height, bool shouldBind) const {
    float w = width * 0.5f;
    float h = height * 0.5f;
    loadShader();
    glDisable(GL_DEPTH_TEST);
    if (shouldBind) bind();
    texture2dShader->use();
    glBindVertexArray(*getVAO());
    texture2dShader->setMat4("proj", orthoProj);
    texture2dShader->setMat4("model", Object::translate(x + w / getWidth() * 0.5f + w * 0.5f, y + h / getHeight(), 0) * Object::scale(w, h, 1));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void cobb::Texture2d::drawRaw(float x, float y, float width, float height, float rotation, bool shouldBind) const {
    float w = width * 0.5f;
    float h = height * 0.5f;
    loadShader();
    glDisable(GL_DEPTH_TEST);
    if (shouldBind) bind();
    texture2dShader->use();
    glBindVertexArray(*getVAO());
    texture2dShader->setMat4("proj", orthoProj);
    texture2dShader->setMat4("model", Object::translate(x + w / getWidth() * 0.5f + w * 0.5f, y + h / getHeight(), 0) * Object::rotate(0, 0, rotation) * Object::scale(w, h, 1));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glEnable(GL_DEPTH_TEST);
}

void cobb::Texture2d::draw(float x, float y, float width, float height, bool shouldBind) const {
    float scale = window->gw / Window::GAME_WIDTH;

    float nx = window->sX(x) + (window->sX(0) * (x / Window::GAME_WIDTH));
    float ny = window->sY(y) + (window->sY(0) * (y / Window::GAME_HEIGHT));
    float nw = width * scale;
    float nh = height * scale;
    drawRaw(nx, ny, nw, nh, shouldBind);
}

void cobb::Texture2d::draw(float x, float y, float width, float height, float rotation, bool shouldBind) const {
    float scale = window->gw / Window::GAME_WIDTH;

    float nx = window->sX(x) + (window->sX(0) * (x / Window::GAME_WIDTH));
    float ny = window->sY(y) + (window->sY(0) * (y / Window::GAME_HEIGHT));
    float nw = width * scale;
    float nh = height * scale;
    drawRaw(nx, ny, nw, nh, rotation, shouldBind);
}

void cobb::Texture2d::setColor(vec4 col) {
    texture2dShader->use();
    texture2dShader->setVec4("color", col);
}

std::string cobb::Texture2d::getPath() const {
    return m_path;
}

cobb::Texture2d::Texture2d() {
    m_path = "";
}
