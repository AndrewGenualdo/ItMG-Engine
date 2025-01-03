//
// Created by cobble on 10/12/2024.
//


#ifndef EWRENDER_CAMERA_HPP
#define EWRENDER_CAMERA_HPP

#pragma once

#include "object.h"
#include <iostream>
#include "window.h"

using namespace cobb;
using namespace glm;

class Camera : public Object {
public:
    constexpr static vec3 right = vec3(1, 0, 0);
    constexpr static vec3 up = vec3(0, 1, 0);
    vec3 forward;
    mat4 view;
    mat4 proj;
    bool lock;
    bool disable;
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

    void setPosRot(std::pair<vec3, vec3> posRot);
    std::string getString();

    void lerpCamera(float startTime, float currentTime, float duration, std::pair<vec3, vec3> startPosRot, std::pair<vec3, vec3> endPosRot);

    static std::pair<vec3, vec3> loadString(const char input[]);

private:
    float lastX, lastY;
    bool firstMouse;
    vec3 startPos;
    vec3 startRot;
    bool wire;

    void load(vec3 pos, vec3 rot, float fov, vec2 screenDims);
};


#endif //EWRENDER_CAMERA_HPP
