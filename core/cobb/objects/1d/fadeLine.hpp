//
// Created by cobble on 10/15/2024.
//

#ifndef EWRENDER_FADELINE_HPP
#define EWRENDER_FADELINE_HPP

#include "line.hpp"

namespace cobb
{

    static Shader *fadeLineShader;

    class FadeLine : public Line {
    public:
        vec4 _color2;

        FadeLine() {
            _start = vec3(0);
            _end = vec3(1);
            _color = vec4(0, 0, 0, 1);
            _color2 = vec4(1);
            load();
        }

        FadeLine(vec3 start, vec3 end, vec4 color, vec4 color2) {
            _start = start;
            _end = end;
            _color = color;
            _color2 = color2;
            load();
        }

        void draw(Camera *camera, bool bind = false) {
            loadShader();
            if(bind) {
                fadeLineShader->use();
                glBindVertexArray(*getVAO());
            }

            fadeLineShader->setMat4("model", Object::translate(_start.x, _start.y, _start.z) * Object::scale(_end.x - _start.x, _end.y - _start.y, _end.z - _start.z));
            fadeLineShader->setVec4("color", camera->lock ? _color * vec4(0.5f, 0.5f, 0.5f, 1.0f) : _color);
            fadeLineShader->setVec4("color2", camera->lock ? _color2 * vec4(0.5f, 0.5f, 0.5f, 1.0f) : _color2);
            glDrawArrays(GL_LINES, 0, 2);
        }
        void draw(vec3 *pos, Camera *camera) {
            fadeLineShader->setMat4("model", Object::translate(_start.x + pos->x, _start.y + pos->y, _start.z + pos->z) * Object::scale((_end.x + pos->x) - (_start.x + pos->x), (_end.y + pos->y) - (_start.y + pos->y), (_end.z + pos->z) - (_start.z + pos->z)));
            fadeLineShader->setVec4("color", camera->lock ? _color * vec4(0.5f, 0.5f, 0.5f, 1.0f) : _color);
            fadeLineShader->setVec4("color2", camera->lock ? _color2 * vec4(0.5f, 0.5f, 0.5f, 1.0f) : _color2);
            glDrawArrays(GL_LINES, 0, 2);
        }

        static void loadShader() {
            if(fadeLineShader == nullptr) {
                cout << "Loaded fade line shader!" << endl;
                fadeLineShader = new Shader("assets/fadeLine");
                fadeLineShader->use();
                glBindVertexArray(*getVAO());
            }
        }

    };

}

#endif //EWRENDER_FADELINE_HPP
