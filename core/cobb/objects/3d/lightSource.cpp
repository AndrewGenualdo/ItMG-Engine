//
// Created by andrew.genualdo on 10/29/2024.
//

#include "lightSource.hpp"

void cobb::LightSource::bind()
{
    glBindVertexArray(*Cube::getVAO());
}

void cobb::LightSource::loadShader()
{
    if(lightShader == nullptr) {
        cout << "Loaded light shader!" << endl;
        lightShader = new Shader("assets/lightSource");
        lightShader->use();
        bind();
    }
}

void cobb::LightSource::draw(Camera &camera)
{
    lightShader->use();
    lightShader->setMat4("viewProj", camera.proj * camera.view);
    lightShader->setVec4("color", color);
    lightShader->setMat4("model", Object::translate(pos.x, pos.y, pos.z) * Object::scale(0.5f, 0.5f, 0.5f));
    LightSource::bind();
    loadShader();
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void LightSource::load()
{
    glBindVertexArray(*Cube::getVAO());
    glBindBuffer(GL_ARRAY_BUFFER, *Cube::getVBO());
    glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_CUBE), VERTICES_CUBE, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)nullptr);
    glEnableVertexAttribArray(0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    loadShader();
}

LightSource::LightSource(vec3 pos, vec4 color)
{
    this->pos = pos;
    this->color = color;
    load();
}
