//
// Created by andrew.genualdo on 9/17/2024.
//
#pragma once

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/texture2d.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>

#include "ew/external/stb_image.h"

using namespace cobb;



int main() {

    const auto window = Window("Hello □");



    //DRAW SQUARE
    auto breadShader = Shader("assets/assignment2/bread.vert", "assets/assignment2/bread.frag");
    breadShader.use();
    breadShader.setInt("tex", 0);
    float breadPositions[] = {0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f};
    Texture2d bread = Texture2d("assets/assignment2/bread.png", &breadShader, GL_NEAREST, GL_REPEAT, breadPositions);

    bread.load();

    auto bgShader = Shader("assets/assignment2/bg.vert", "assets/assignment2/bg.frag");
    bgShader.use();
    bgShader.setInt("tex", 0);
    bgShader.setInt("tex2", 1);
    float bgPositions[] = {2.5f, 2.5f, 2.5f, -2.5f, -2.5f, -2.5f, -2.5f, 2.5f};
    Texture2d bg = Texture2d("assets/assignment2/bg.png", &bgShader, GL_LINEAR, GL_REPEAT, bgPositions);



    float repeat = 20;
    // this might look random, but I have all the indexes linked to what they actually do in texture2d.cpp in the loadVertices function.
    // Depending on how much I end up doing this, I might make a constant for each index
    // So I can do bg.setVertex(TOP_LEFT_X, value) or something
    /*bg.vertices[7] = repeat;
    bg.vertices[8] = repeat;
    bg.vertices[16] = repeat;
    bg.vertices[35] = repeat;*/
    bg.load();

    Texture2d bg2 = Texture2d("assets/assignment2/bg2.png", &bgShader, GL_LINEAR, GL_REPEAT, bgPositions);
    bg2.vertices[7] = repeat;
    bg2.vertices[8] = repeat;
    bg2.vertices[16] = repeat;
    bg2.vertices[35] = repeat;
    bg2.load();


    glBindVertexArray(*Texture2d::getVAO());

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        //Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing happens here!
        float time = static_cast<float>(glfwGetTime());

        bgShader.use();
        glActiveTexture(GL_TEXTURE0);
        bg.bind();
        glActiveTexture(GL_TEXTURE1);
        bg2.bind();
        bgShader.setFloat("time", time);
        bg.draw();

        breadShader.use();
        glActiveTexture(GL_TEXTURE0);
        bread.bind();
        breadShader.setFloat("time", time);
        bread.draw();

        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
    glfwTerminate();
    return 0;
}