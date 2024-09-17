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

using namespace cobb;


int assignment2() {

    const auto window = Window("Hello □");

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        //Clear framebuffer
        glClear(GL_COLOR_BUFFER_BIT);
        //Drawing happens here!





        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");

    return 0;
}
