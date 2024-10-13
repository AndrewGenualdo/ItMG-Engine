//
// Created by andrew.genualdo on 9/17/2024.
//
#pragma once

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/objects/2d/texture2d.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>

#include "ew/external/stb_image.h"

using namespace cobb;



int main() {

    Window window = Window("Hello □");



    //DRAW SQUARE
    auto breadShader = Shader("assets/assignment2/bread.vert", "assets/assignment2/bread.frag");
    breadShader.use();
    breadShader.setInt("tex", 0);
    float breadPositions[] = {0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f};
    Texture2d bread = Texture2d("assets/assignment2/bread.png", GL_NEAREST, GL_REPEAT, breadPositions);

    auto bgShader = Shader("assets/assignment2/bg.vert", "assets/assignment2/bg.frag");
    bgShader.use();
    bgShader.setInt("tex", 0);
    bgShader.setInt("tex2", 1);
    float bgPositions[] = {2.5f, 2.5f, 2.5f, -2.5f, -2.5f, -2.5f, -2.5f, 2.5f};
    Texture2d bg = Texture2d("assets/assignment2/bg.png", GL_LINEAR, GL_REPEAT, bgPositions);
    Texture2d bg2 = Texture2d("assets/assignment2/bg2.png", GL_LINEAR, GL_REPEAT, bgPositions);
    
    glBindVertexArray(*Texture2d::getVAO());

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        window.update();

        //Drawing happens here!
        const auto time = static_cast<float>(glfwGetTime());


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