//
// Created by cobble on 10/12/2024.
//

#include "camera.hpp"

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
    if(lock) return;
    //angle
    vec3 dir;
    dir.x = cos(radians(_rotation.z)) * cos(radians(_rotation.y));
    dir.y = sin(radians(_rotation.y));
    dir.z = sin(radians(_rotation.z)) * cos(radians(_rotation.y));
    forward = normalize(dir);

    view = mat4(1);
    view = glm::lookAt(_position, _position + forward, up);

    //position
    float speedMult = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? 25.0f : glfwGetKey(window, GLFW_KEY_LEFT_ALT) ? 1.0f : 10.0f;
    //if(glfwGetKey(window, GLFW_KEY_W)) position += deltaTime * speedMult * forward;
    //if(glfwGetKey(window, GLFW_KEY_S)) position -= deltaTime * speedMult * forward;
    if(glfwGetKey(window, GLFW_KEY_W)) _position += deltaTime * speedMult * normalize(cross(up, cross(forward, up)));
    if(glfwGetKey(window, GLFW_KEY_S)) _position -= deltaTime * speedMult * normalize(cross(up, cross(forward, up)));
    if(glfwGetKey(window, GLFW_KEY_D)) _position += deltaTime * speedMult * normalize(cross(forward, up));
    if(glfwGetKey(window, GLFW_KEY_A)) _position -= deltaTime * speedMult * normalize(cross(forward, up));
    if(glfwGetKey(window, GLFW_KEY_Q) || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) _position -= deltaTime * speedMult * up;
    if(glfwGetKey(window, GLFW_KEY_E) || glfwGetKey(window, GLFW_KEY_SPACE)) _position += deltaTime * speedMult * up;

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
    _position = startPos;
    _rotation = startRot;
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


    if(!lock) {
        const float sensitivity = 0.1f;
        xOff *= sensitivity;
        yOff *= sensitivity;
        _rotation.z += xOff;
        _rotation.y += yOff;
    }

    if(_rotation.y > 89.0f) {
        _rotation.y = 89.0f;
    }
    if(_rotation.y < -89.0f) {
        _rotation.y = -89.0f;
    }
}

void Camera::handleKeyboard(int key, int action) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) {
            lock = !lock;
        } else if(key == GLFW_KEY_F1) {
            ui = !ui;
        } else if(key == GLFW_KEY_P) {
            cout << "Position: " << _position.x << " " << _position.y << " " << _position.z << endl;
            cout << "Rotation: " << _rotation.x << " " << _rotation.y << " " << _rotation.z << endl;
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
    _position = pos;
    startRot = rot;
    _rotation = rot;
    forward = vec3(0, 0, -1);
    view = mat4(1);
    lock = false;
    FOV = fov;
    proj = mat4(1);
    proj = perspective(radians(fov), (float) Window::SCREEN_WIDTH / (float) Window::SCREEN_HEIGHT, 0.1f, 1000.0f);
    ui = true;
}
