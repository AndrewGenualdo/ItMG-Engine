//
// Created by cobble on 10/13/2024.
//

#include "line.h"

unsigned int *cobb::Line::getVAO()
{
    if (LINE_VAO == -1) {
        glGenVertexArrays(1, &LINE_VAO);
    }

    return &LINE_VAO;
}

unsigned int *cobb::Line::getVBO()
{
    if (LINE_VBO == -1) {
        glGenBuffers(1, &LINE_VBO);
    }

    return &LINE_VBO;
}
