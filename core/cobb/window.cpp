//
// Created by andrew.genualdo on 9/17/2024.
//

#include "window.hpp"

#include "GLFW/glfw3.h"


cobb::Window::Window(const string& title) {
    window = nullptr;
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
    glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
}
