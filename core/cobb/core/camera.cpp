//
// Created by cobble on 10/12/2024.
//

#include <vector>
#include "camera.h"

Camera::Camera() {
    load(vec3(0), vec3(0), 60.0f, vec2(1920, 1080));
}

Camera::Camera(vec2 screenDims) {
    load(vec3(0), vec3(0), 60.0f, screenDims);
}

Camera::Camera(vec3 position, vec2 screenDims) {
    load(position, vec3(0), 60.0f, screenDims);
}

Camera::Camera(vec3 position, vec3 lookingAt, vec2 screenDims) {
    load(position, lookingAt, 60.0f, screenDims);
}

Camera::Camera(vec3 position, vec3 lookingAt, float fov, vec2 screenDims) {
    load(position, lookingAt, fov, screenDims);
}

void Camera::update(GLFWwindow* window, float deltaTime) {

    //angle
    vec3 dir;
    dir.x = cos(radians(rot.z)) * cos(radians(rot.y));
    dir.y = sin(radians(rot.y));
    dir.z = sin(radians(rot.z)) * cos(radians(rot.y));
    forward = normalize(dir);

    view = mat4(1);
    view = glm::lookAt(pos, pos + forward, up);

    //position
    float speedMult = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? 25.0f : glfwGetKey(window, GLFW_KEY_LEFT_ALT) ? 1.0f : 10.0f;
    //if(glfwGetKey(window, GLFW_KEY_W)) position += deltaTime * speedMult * forward;
    //if(glfwGetKey(window, GLFW_KEY_S)) position -= deltaTime * speedMult * forward;
    if(!disable && !lock) {
        if(glfwGetKey(window, GLFW_KEY_W)) pos += deltaTime * speedMult * normalize(cross(up, cross(forward, up)));
        if(glfwGetKey(window, GLFW_KEY_S)) pos -= deltaTime * speedMult * normalize(cross(up, cross(forward, up)));
        if(glfwGetKey(window, GLFW_KEY_D)) pos += deltaTime * speedMult * normalize(cross(forward, up));
        if(glfwGetKey(window, GLFW_KEY_A)) pos -= deltaTime * speedMult * normalize(cross(forward, up));
        if(glfwGetKey(window, GLFW_KEY_Q) || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) pos -= deltaTime * speedMult * up;
        if(glfwGetKey(window, GLFW_KEY_E) || glfwGetKey(window, GLFW_KEY_SPACE)) pos += deltaTime * speedMult * up;
    }

    float n = 0.1f;
    float f = 1000.0f;
    //fov
    //proj = mat4(1);
    float ntan = 1.0f / tan(radians(FOV) / 2.0f);
    float aspectRatio = static_cast<float>(Window::SCREEN_WIDTH) / static_cast<float>(Window::SCREEN_HEIGHT);
    /*proj[0][0] = ntan / aspectRatio;
    proj[1][1] = ntan;
    proj[2][2] = (n + f) / (n - f);
    proj[2][3] = -1;
    proj[3][2] = (2.0f * f * n)/(n - f);
    proj[3][3] = 0;*/
    proj = mat4(
        ntan / aspectRatio, 0, 0, 0,
        0, ntan, 0, 0,
        0, 0, (n + f) / (n - f), -1,
        0, 0, (2.0f * f * n)/(n - f), 0
    );
    //proj = perspective(radians(FOV), aspectRatio, n, f);
    /*

     */

}

void Camera::reset() {
    pos = startPos;
    rot = startRot;
}

void Camera::handleMouse(double xPos, double yPos) {

    if(firstMouse) {
        lastX = static_cast<float>(xPos);
        lastY = static_cast<float>(yPos);
        firstMouse = false;
    }

    auto xOff = static_cast<float>(xPos - lastX);
    auto yOff = static_cast<float>(lastY - yPos);
    lastX = static_cast<float>(xPos);
    lastY = static_cast<float>(yPos);


    if(!lock && !disable) {
        const float sensitivity = 0.1f;
        xOff *= sensitivity;
        yOff *= sensitivity;
        rot.z += xOff;
        rot.y += yOff;
    }

    if(rot.y > 89.0f) {
        rot.y = 89.0f;
    }
    if(rot.y < -89.0f) {
        rot.y = -89.0f;
    }
}

void Camera::handleKeyboard(int key, int action) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) {
            lock = !lock;
        } else if(key == GLFW_KEY_F1) {
            ui = !ui;
        } else if(key == GLFW_KEY_F3) {
            if(wire) {
                glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
            }
            wire = !wire;
        }
    }
}

void Camera::handleScroll(double yOff) {
    FOV -= static_cast<float>(yOff);
    if (FOV < 1.0f) FOV = 1.0f;
    if (FOV > 120.0f) FOV = 120.0f;
}

void Camera::load(vec3 pos, vec3 rot, float fov, vec2 screenDims) {
    firstMouse = true;
    lastX = screenDims.x / 2.0f;
    lastY = screenDims.y / 2.0f;
    startPos = pos;
    this->pos = pos;
    startRot = rot;
    this->rot = rot;
    forward = vec3(0, 0, -1);
    view = mat4(1);
    lock = false;
    disable = false;
    FOV = fov;
    proj = mat4(1);
    proj = perspective(radians(fov), (float) Window::SCREEN_WIDTH / (float) Window::SCREEN_HEIGHT, 0.1f, 1000.0f);
    ui = true;
    wire = false;
}

//Taken from https://stackoverflow.com/a/46931770
std::vector<std::string> split(std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

std::pair<vec3, vec3> Camera::loadString(const char input[]) {
    vec3 pos;
    vec3 rot;
    std::string inp = input;
    std::vector<std::string> result = split(inp.substr(1, inp.length() - 1), ", ");
    int i = 0;
    for(const auto& value : result) {
        switch(i) {
            case 0: pos.x = stof(value); break;
            case 1: pos.y = stof(value); break;
            case 2: pos.z = stof(value); break;
            case 3: rot.x = stof(value); break;
            case 4: rot.y = stof(value); break;
            case 5: rot.z = stof(value); break;
            default: break;
        }
        i++;
    }
    return std::make_pair(pos, rot);
}

std::string Camera::getString() {
    return "[" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z) + ", " + std::to_string(rot.x) + ", " + std::to_string(rot.y) + ", " + std::to_string(rot.z) + "]";
}

void Camera::setPosRot(std::pair<vec3, vec3> posRot) {
    pos = posRot.first;
    rot = posRot.second;
}

static float lerp(float a, float b, float t)
{
    return a + (b - a) * t; //This returns a + t percent (t = 0.f is 0% and t = 1.f is 100%) of b
}

void Camera::lerpCamera(float startTime, float currentTime, float duration, std::pair<vec3, vec3> startPosRot, std::pair<vec3, vec3> endPosRot) {


    float percentThrough = (currentTime - startTime) / duration;
    pos.x = lerp(startPosRot.first.x, endPosRot.first.x, percentThrough);
    pos.y = lerp(startPosRot.first.y, endPosRot.first.y, percentThrough);
    pos.z = lerp(startPosRot.first.z, endPosRot.first.z, percentThrough);
    rot.x = lerp(startPosRot.second.x, endPosRot.second.x, percentThrough);
    rot.y = lerp(startPosRot.second.y, endPosRot.second.y, percentThrough);
    rot.z = lerp(startPosRot.second.z, endPosRot.second.z, percentThrough);

    if(currentTime - startTime > duration) {
        pos = endPosRot.first;
        rot = endPosRot.second;
    }

}
