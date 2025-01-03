//
// Created by andrew.genualdo on 9/17/2024.
//

#include "window.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "GLFW/glfw3.h"


cobb::Window::Window(const std::string &title) {
    window = nullptr;
    _time = 0;
    _uptime = 0;
    _title = title;
    _frames = 0;
    _width = SCREEN_WIDTH;
    _height = SCREEN_HEIGHT;
    _timeOffset = 0;
    mousePos = glm::vec2(0);
    std::cout << "Initializing Window..." << std::endl;
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
    glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    getGameDimensions();
}

float cobb::Window::update() {
    //show fps/uptime in window title
    const auto time = static_cast<float>(glfwGetTime() - _timeOffset);
    const float deltaTime = time - _time;
    _time = time;
    if (static_cast<int>(_time) > _uptime) {
        glfwSetWindowTitle(window, (_title + " | FPS: " + std::to_string(_frames) + " | " + std::to_string(_uptime) +
                                    "s").c_str());
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

float cobb::Window::getWidth() const {
    return _width;
}

float cobb::Window::getHeight() const {
    return _height;
}

void cobb::Window::setWidth(int width) {
    _width = width;
}

void cobb::Window::setHeight(int height) {
    _height = height;
}

cobb::Window::Window() {

}

void cobb::Window::setVsync(bool vsync) {
    if (vsync) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

/*
 * Converts screen x coordinates into game x coordinates
 */
float cobb::Window::gX(float screenX) const {
    return GAME_WIDTH * (screenX - gx) / gw;
}

/*
 * Converts screen y coordinates into game y coordinates
 */
float cobb::Window::gY(float screenY) const {
    return GAME_HEIGHT * (screenY - gy) / gh;
}

/*
 * Converts game x coordinates into screen x coordinates
 */
float cobb::Window::sX(float gameX) const {
    return (gw - gx) * gameX / GAME_WIDTH + gx;
}

/*
 * Converts game y coordinates into screen y coordinates
 */
float cobb::Window::sY(float gameY) const {
    return (gh - gy) * gameY / GAME_HEIGHT + gy;
}

void cobb::Window::getGameDimensions() {
    float aspectScreen = getWidth() / getHeight();
    float aspectGame = GAME_WIDTH / GAME_HEIGHT;

    // Default values for x and y are top-left corner (0,0)
    float x = 0.0f;
    float y = 0.0f;
    float width;
    float height;

    if (aspectScreen > aspectGame) {
        // Screen is wider than the game (horizontal black bars), scale by height
        height = getHeight();  // Set the height as the screen's height
        width = height * aspectGame;  // Adjust width to maintain 16:9 aspect ratio
        x = (getWidth() - width) / 2.0f;  // Center horizontally
    } else {
        // Screen is taller than the game (vertical black bars), scale by width
        width = getWidth();  // Set the width as the screen's width
        height = width / aspectGame;  // Adjust height to maintain 16:9 aspect ratio
        y = (getHeight() - height) / 2.0f;  // Center vertically
    }

    // Ensuring the texture fits within the screen bounds
    if (x < 0) x = 0;  // Prevent negative x offset
    if (y < 0) y = 0;  // Prevent negative y offset
    if (width > getWidth()) width = getWidth();  // Prevent width from exceeding screen width
    if (height > getHeight()) height = getHeight();  // Prevent height from exceeding screen height
    gx = x;
    gy = y;
    gw = width;
    gh = height;

}

void cobb::Window::setTime(double time) {
    glfwSetTime(time);
    _timeOffset = time;
}


