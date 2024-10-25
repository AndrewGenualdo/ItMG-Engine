//
// Created by andrew.genualdo on 9/17/2024.
//

#include "window.hpp"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "GLFW/glfw3.h"


cobb::Window::Window(const string& title) {
    window = nullptr;
    _uptime = 0;
    _title = title;
    _frames = 0;
    cout << "Initializing Window..." << endl;
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
    glEnable(GL_DEPTH_TEST);
}

float cobb::Window::update() {
    //show fps/uptime in window title
    const auto time = static_cast<float>(glfwGetTime());
    const float deltaTime = time - _time;
    _time = time;
    if(static_cast<int>(_time) > _uptime) {
        glfwSetWindowTitle(window, (_title + " | FPS: " + to_string(_frames) + " | " + to_string(_uptime)+"s").c_str());
        _frames = 0;
        _uptime++;
    }
    _frames++;

    //Clear framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    return deltaTime;
}

float cobb::Window::getTime() const {
    return _time;
}

