//
// Created by cobble on 10/13/2024.
//

#ifndef EWRENDER_LINE_HPP
#define EWRENDER_LINE_HPP

#pragma once

#include "../core/camera.h"
#include <string>
#include <filesystem>
#include "../../ew/external/glad.h"
#include "../../ew/external/stb_image.h"
#include "../core/shader.h"
#include "glm/detail/type_vec3.hpp"
#include "glm/detail/type_vec4.hpp"

using namespace glm;

namespace cobb {

    static unsigned int LINE_VAO = -1, LINE_VBO = -1;

    const static float VERTICES_LINE[] = {
            0, 0, 0,
            1, 1, 1,
    };

    static Shader *lineShader;

    class Line {

    public:

        static unsigned int* getVAO();
        static unsigned int* getVBO();

        vec3 _start;
        vec3 _end;
        vec4 _color;

        Line() {
            _start = vec3(0);
            _end = vec3(1);
            _color = vec4(1);
            load();
        }

        Line(vec3 start, vec3 end, vec4 color)
        {
            _start = start;
            _end = end;
            _color = color;
            load();
        }

        void load() {
            loadShader();
            lineShader->use();

            glBindVertexArray(*getVAO());

            glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_LINE), VERTICES_LINE, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

        }

        virtual void draw(Camera *camera, bool bind = false) {
            loadShader();
            if(bind) {
                lineShader->use();
                glBindVertexArray(*getVAO());
            }

            lineShader->setMat4("model", Object::translate(_start.x, _start.y, _start.z) * Object::scale(_end.x - _start.x, _end.y - _start.y, _end.z - _start.z));
            lineShader->setVec4("color", camera->lock ? _color * vec4(0.5f, 0.5f, 0.5f, 1.0f) : _color);
            glDrawArrays(GL_LINES, 0, 2);
        }

        virtual void draw(vec3 *pos, Camera *camera) {
            lineShader->setMat4("model", Object::translate(_start.x + pos->x, _start.y + pos->y, _start.z + pos->z) * Object::scale((_end.x + pos->x) - (_start.x + pos->x), (_end.y + pos->y) - (_start.y + pos->y), (_end.z + pos->z) - (_start.z + pos->z)));
            lineShader->setVec4("color", camera->lock ? _color * vec4(0.5f, 0.5f, 0.5f, 1.0f) : _color);
            glDrawArrays(GL_LINES, 0, 2);
        }

        static void loadShader() {
            if(lineShader == nullptr) {
                std::cout << "Loaded line shader!" << std::endl;
                lineShader = new Shader("assets/line");
                lineShader->use();
                glBindVertexArray(*getVAO());
            }
        }
    };
}



#endif //EWRENDER_LINE_HPP
