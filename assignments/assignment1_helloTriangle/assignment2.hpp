//
// Created by andrew.genualdo on 9/17/2024.
//
#pragma once

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>

#include "ew/external/stb_image.h"

using namespace cobb;



int assignment2() {

    const float vertices[] = {
     // positions (xyz)      color (rgba)              texture coords (xy) ((0,0) is bottom left)
        0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,// top right
        0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,// bottom right
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,// bottom left
       -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f,// top left
    };
    const unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    const auto window = Window("Hello □");

    //DRAW SQUARE
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //coordinates
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    //color
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //texture coords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float),reinterpret_cast<void *>(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);




    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("assets/assignment2/bread.png", &width, &height, &nrChannels, 0);

    if(data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        cout << "Failed to load texture!! " << endl;
    }

    auto shader = Shader("assets/template/template.vert", "assets/template/template.frag");
    shader.use();


    stbi_image_free(data);


    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        //Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing happens here!

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}
