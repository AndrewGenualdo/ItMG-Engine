//
// Created by cobble on 10/12/2024.
//

#include "camera.hpp"

Camera::Camera() {
    load(vec3(0), vec3(0), 45.0f, vec2(1920, 1080));
}

Camera::Camera(vec2 screenDims) {
    load(vec3(0), vec3(0), 45.0f, screenDims);
}

Camera::Camera(vec3 position, vec2 screenDims) {
    load(position, vec3(0), 45.0f, screenDims);
}

Camera::Camera(vec3 position, vec3 lookingAt, vec2 screenDims) {
    load(position, lookingAt, 45.0f, screenDims);
}

Camera::Camera(vec3 position, vec3 lookingAt, float fov, vec2 screenDims) {
    load(position, lookingAt, fov, screenDims);
}

void Camera::update(GLFWwindow* window, float deltaTime) {
    //angle
    vec3 dir;
    dir.x = cos(radians(rotation.z)) * cos(radians(rotation.y));
    dir.y = sin(radians(rotation.y));
    dir.z = sin(radians(rotation.z)) * cos(radians(rotation.y));
    cameraFront = normalize(dir);

    view = mat4(1);
    view = glm::lookAt(position, position + cameraFront, up);

    //position
    float speedMult = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) ? 25.0f : 10.0f;
    if(glfwGetKey(window, GLFW_KEY_W)) position += deltaTime * speedMult * cameraFront;
    if(glfwGetKey(window, GLFW_KEY_S)) position -= deltaTime * speedMult * cameraFront;
    if(glfwGetKey(window, GLFW_KEY_D)) position += deltaTime * speedMult * normalize(cross(cameraFront, up));
    if(glfwGetKey(window, GLFW_KEY_A)) position -= deltaTime * speedMult * normalize(cross(cameraFront, up));
    if(glfwGetKey(window, GLFW_KEY_Q) || glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) position -= deltaTime * speedMult * up;
    if(glfwGetKey(window, GLFW_KEY_E) || glfwGetKey(window, GLFW_KEY_SPACE)) position += deltaTime * speedMult * up;

    //fov
    proj = mat4(1);
    proj = perspective(radians(FOV), (float) Window::SCREEN_WIDTH / (float) Window::SCREEN_HEIGHT, 0.1f, 1000.0f);
}

void Camera::reset() {
    position = startPos;
    rotation = startRot;
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
        rotation.z += xOff;
        rotation.y += yOff;
    }

    if(rotation.y > 89.0f) {
        rotation.y = 89.0f;
    }
    if(rotation.y < -89.0f) {
        rotation.y = -89.0f;
    }
}

void Camera::handleKeyboard(int key, int action) {
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_P) {
            cout << "Position: " << position.x << " " << position.y << " " << position.z << endl;
            cout << "Rotation: " << rotation.x << " " << rotation.y << " " << rotation.z << endl;
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
    position = pos;
    startRot = rot;
    rotation = rot;
    cameraFront = vec3(0, 0, -1);
    view = mat4(1);
    lock = false;
    FOV = fov;
    proj = mat4(1);
    proj = perspective(radians(fov), (float) Window::SCREEN_WIDTH / (float) Window::SCREEN_HEIGHT, 0.1f, 1000.0f);
}
