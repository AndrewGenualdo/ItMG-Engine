//
// Created by andrew.genualdo on 10/29/2024.
//

#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "cube.hpp"

using namespace std;
using namespace glm;

namespace cobb {

    static Shader *lightShader;


    class LightSource {

    public:
        vec3 pos;
        vec4 color;

        LightSource(vec3 pos, vec4 color) {
            this->pos = pos;
            this->color = color;
            load();
        }

        void load() {
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

        void draw() {
            loadShader();
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        static void loadShader() {
            if(lightShader == nullptr) {
                cout << "Loaded light shader!" << endl;
                lightShader = new Shader("assets/lightSource");
                lightShader->use();
                glBindVertexArray(*Cube::getVAO());
            }
        }
    };
}

#endif //LIGHTSOURCE_H
