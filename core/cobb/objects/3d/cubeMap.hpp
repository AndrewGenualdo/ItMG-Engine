//
// Created by andrew.genualdo on 11/8/2024.
//

#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "iostream"
#include <string>
#include "../../../ew/external/glad.h"
#include "../../../ew/external/stb_image.h"
#include "../../shader.hpp"
namespace cobb {
    static unsigned int CUBEMAP_VAO = -1, CUBEMAP_VBO = -1;

    class CubeMap {

        unsigned int m_id;

        int m_width, m_height;
        int m_filterMode;
        const char *m_path;

    public:


        CubeMap() {

        }

        void load() {

        }

        void bind() {
            glBindTexture(GL_TEXTURE_2D, m_id);
        }

        void draw() {
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        unsigned int getId() const {
            return m_id;
        }

        static unsigned int* getVAO() {
            if (CUBEMAP_VAO == -1) {
                glGenVertexArrays(1, &CUBEMAP_VAO);
            }

            return &CUBEMAP_VAO;
        }
        static unsigned int* getVBO() {
            if (CUBEMAP_VBO == -1) {
                glGenBuffers(1, &CUBEMAP_VBO);
            }

            return &CUBEMAP_VBO;
        }

    };
}




#endif //CUBEMAP_H
