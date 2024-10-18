//
// Created by cobble on 10/15/2024.
//

#include "triangle.hpp"

unsigned int *cobb::Triangle::getVAO()
{
    if (TRIANGLE_VAO == -1) {
        glGenVertexArrays(1, &TRIANGLE_VAO);
    }

    return &TRIANGLE_VAO;
}

unsigned int *cobb::Triangle::getVBO()
{
    if (TRIANGLE_VBO == -1) {
        glGenBuffers(1, &TRIANGLE_VBO);
    }

    return &TRIANGLE_VBO;
}
