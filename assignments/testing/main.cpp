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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace cobb;
using namespace glm;


int main() {

    const auto window = Window("Hello □");



    /*//DRAW SQUARE
    auto breadShader = Shader("assets/assignment2/bread");
    breadShader.use();
    breadShader.setInt("tex", 0);
    float breadPositions[] = {0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, 0.5f};
    Texture2d bread = Texture2d("assets/assignment2/bread.png", GL_NEAREST, GL_REPEAT, breadPositions);

    auto bgShader = Shader("assets/assignment2/bg");
    bgShader.use();
    bgShader.setInt("tex", 0);
    bgShader.setInt("tex2", 1);
    float bgPositions[] = {2.5f, 2.5f, 2.5f, -2.5f, -2.5f, -2.5f, -2.5f, 2.5f};
    Texture2d bg = Texture2d("assets/assignment2/bg.png", GL_LINEAR, GL_REPEAT, bgPositions);
    Texture2d bg2 = Texture2d("assets/assignment2/bg2.png", GL_LINEAR, GL_REPEAT, bgPositions);
    
    glBindVertexArray(*Texture2d::getVAO());*/

    auto testShader = Shader("assets/testing/testing");
    testShader.use();
    testShader.setInt("tex", 0);
    testShader.setVec2("dims", vec2(cobb::Window::SCREEN_WIDTH, cobb::Window::SCREEN_HEIGHT));
    float screenCorners[] = {1, 1, 1, -1, -1, -1, -1, 1};
    Texture2d errorTexture = Texture2d("assets/assignment2/bg.png", GL_LINEAR, GL_REPEAT, screenCorners);
    glBindVertexArray(*Texture2d::getVAO());
    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        //Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing happens here!
        const auto time = static_cast<float>(glfwGetTime());
        //glActiveTexture(GL_TEXTURE0);
        testShader.use();
        errorTexture.bind();
        testShader.setFloat("time", time);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


        /*bgShader.use();

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
        bread.draw();*/



        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
    glfwTerminate();
    return 0;
}