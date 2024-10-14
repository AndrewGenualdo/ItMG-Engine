//
// Created by cobble on 10/12/2024.
//


#ifndef EWRENDER_CAMERA_HPP
#define EWRENDER_CAMERA_HPP

#pragma once

#include "object.hpp"
#include <iostream>
#include "../../window.hpp"

using namespace std;
using namespace cobb;
using namespace glm;

class Camera : public Object {
public:
    constexpr static vec3 right = vec3(1, 0, 0);
    constexpr static vec3 up = vec3(0, 1, 0);
    vec3 cameraFront;
    mat4 view;
    mat4 proj;
    bool lock;
    bool ui;
    float FOV;

    Camera();
    Camera(vec2 screenDims);
    Camera(vec3 position, vec2 screenDims);
    Camera(vec3 position, vec3 lookingAt, vec2 screenDims);
    Camera(vec3 position, vec3 lookingAt, float fov, vec2 screenDims);

    void update(GLFWwindow* window, float deltaTime);
    void reset();

    void handleMouse(double xPos, double yPos);
    void handleKeyboard(int key, int action);
    void handleScroll(double yOff);

private:
    float lastX, lastY;
    bool firstMouse;
    vec3 startPos;
    vec3 startRot;

    void load(vec3 pos, vec3 rot, float fov, vec2 screenDims);
};


#endif //EWRENDER_CAMERA_HPP
