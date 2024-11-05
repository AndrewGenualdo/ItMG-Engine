//
// Created by cobble on 10/16/2024.
//

#ifndef EWRENDER_FADETRIANGLE_H
#define EWRENDER_FADETRIANGLE_H

#include "triangle.hpp"

namespace cobb
{

    static Shader *fadeTriangleShader;

    class FadeTriangle : public Triangle {
    public:
        vec4 color2, color3;

        FadeTriangle() {
            p1 = vec3(0, 0, 0);
            p2 = vec3(1, 0, 0);
            p3 = vec3(0.5f, 0.5f, 0);
            color = vec4(1, 0, 0, 1);
            color2 = vec4(0, 1, 0, 1);
            color3 = vec4(0, 0, 1, 1);
            loadFade();
        }

        FadeTriangle(vec3 _p1, vec3 _p2, vec3 _p3, vec4 _color, vec4 _color2, vec4 _color3) {
            p1 = _p1;
            p2 = _p2;
            p3 = _p3;
            color = _color;
            color2 = _color2;
            color3 = _color3;
            loadFade();
        }

        void loadFade() {
            loadShader();

            fadeTriangleShader->use();

            glBindVertexArray(*getVAO());

            glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES_TRIANGLE), VERTICES_TRIANGLE, GL_STREAM_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void draw(Camera *camera, bool bind = false) {
            loadShader();
            if(bind) {
                fadeTriangleShader->use();
                glBindVertexArray(*getVAO());
            }

            const vec3 targetPoints[3] = { p1, p2, p3 };

            /*constexpr vec3 originalPoints[3] = {
                vec3(0, 0, 0),
                vec3(1, 0, 0),
                vec3(0, 1, 0)
            };*/

            //constexpr vec3 oV0 = originalPoints[1] - originalPoints[0];
            //constexpr vec3 oV1 = originalPoints[2] - originalPoints[0];
            //const auto originalBasis = mat3(oV0, oV1, cross(oV0, oV1));

            //const mat3 invOriginalBasis = inverse(originalBasis);
            //constexpr auto invOriginalBasis = mat3(1);

            const vec3 tV0 = targetPoints[1] - targetPoints[0];
            const vec3 tV1 = targetPoints[2] - targetPoints[0];
            const auto targetBasis = mat3(tV0, tV1, cross(tV0, tV1));

            //const mat3 rotationScaling = targetBasis;// invOriginalBasis;

            //glBindBuffer(GL_ARRAY_BUFFER, *getVBO());
            //glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(dynamicVertices), dynamicVertices);
            //glDrawArrays(GL_TRIANGLES, 0, 3);

            fadeTriangleShader->setMat4("model", Object::translate(p1.x, p1.y, p1.z) * mat4(targetBasis));
            fadeTriangleShader->setVec4("color", camera->lock ? color * vec4(0.5f, 0.5f, 0.5f, 1.0f) : color);
            fadeTriangleShader->setVec4("color2", camera->lock ? color2 * vec4(0.5f, 0.5f, 0.5f, 1.0f) : color2);
            fadeTriangleShader->setVec4("color3", camera->lock ? color3 * vec4(0.5f, 0.5f, 0.5f, 1.0f) : color3);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }


        static void loadShader() {
            if(fadeTriangleShader == nullptr) {
                cout << "Loaded fade triangle shader!" << endl;
                fadeTriangleShader = new Shader("assets/fadeTriangle");
                fadeTriangleShader->use();
                glBindVertexArray(*getVAO());
            }
        }
    };

}

#endif //EWRENDER_FADETRIANGLE_H
