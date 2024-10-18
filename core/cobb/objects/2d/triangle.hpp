//
// Created by cobble on 10/15/2024.
//

#ifndef EWRENDER_TRIANGLE_HPP
#define EWRENDER_TRIANGLE_HPP

#pragma once

#include <iostream>
#include "../../../ew/external/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "../../shader.hpp"
#include "../3d/camera.hpp"

using namespace std;
using namespace glm;

namespace cobb {

    static unsigned int TRIANGLE_VAO = -1, TRIANGLE_VBO = -1;

    const static float VERTICES_TRIANGLE[] = {
            0, 0, 0,
            1, 0, 0,
            0, 0, 1
    };

    static Shader *triangleShader;

    class Triangle
    {
    public:

        static unsigned int* getVAO();
        static unsigned int* getVBO();

        vec3 p1, p2, p3;
        vec4 color;

        Triangle() {
            p1 = vec3(0, 0, 0);
            p2 = vec3(1, 0, 0);
            p3 = vec3(0.5f, 0.5f, 0);
            color = vec4(1);
            load();
        }

        Triangle(vec3 _p1, vec3 _p2, vec3 _p3, vec4 _color) {
            p1 = _p1;
            p2 = _p2;
            p3 = _p3;
            color = _color;
            load();
        }

        void load() {
            loadShader();


            triangleShader->use();

            glBindVertexArray(*getVAO());

            glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_TRIANGLE), VERTICES_TRIANGLE, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void draw(Camera *camera, bool bind = false) {
            loadShader();
            if(bind) {
                triangleShader->use();
                glBindVertexArray(*getVAO());
            }

            vec3 dynamicVertices[3] = {
                    p1, p2, p3
            };

            //glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(dynamicVertices), dynamicVertices);
            const vec3 side1 = normalize(p2 - p1);
            const vec3 side2 = normalize(p3 - p1);
            mat4 scale = mat4(1);
            scale[0] = vec4(side1.x, side1.y, side1.z, 0);
            scale[2] = vec4(side2.x, side2.y, side2.z, 0);
            triangleShader->setMat4("model", Object::translate(p1.x, p1.y, p1.z) * scale);
            triangleShader->setVec4("color", camera->lock ? color * vec4(0.5f, 0.5f, 0.5f, 1.0f) : color);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        static void loadShader() {
            if(triangleShader == nullptr) {
                cout << "Loaded triangle shader!" << endl;
                triangleShader = new Shader("assets/triangle");
                triangleShader->use();
                glBindVertexArray(*getVAO());
            }
        }
    };
}



#endif //EWRENDER_TRIANGLE_HPP
