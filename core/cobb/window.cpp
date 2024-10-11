//
// Created by andrew.genualdo on 9/17/2024.
//

#include "window.hpp"

#include "GLFW/glfw3.h"


cobb::Window::Window(const string& title) {
    window = nullptr;
    _uptime = 0;
    _title = title;
    _frames = 0;
    printf("Initializing...");
    if (!glfwInit()) {
        printf("GLFW failed to init!");
        return;
    }
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title.c_str(), nullptr, nullptr);
    if (window == nullptr) {
        printf("GLFW failed to create window");
        return;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGL(glfwGetProcAddress)) {
        printf("GLAD Failed to load GL headers");
        return;
    }
    glfwSwapInterval(0);
    glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
}

void cobb::Window::update() {
    //show fps/uptime in window title
    if(static_cast<int>(glfwGetTime()) > _uptime) {
        glfwSetWindowTitle(window, (_title + " | FPS: " + to_string(_frames) + " | " + to_string(_uptime)+"s").c_str());
        _frames = 0;
        _uptime++;
    }
    _frames++;
    //Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT);
}

