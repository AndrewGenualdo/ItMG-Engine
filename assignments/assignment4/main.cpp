//
// Created by andrew.genualdo on 9/17/2024.
//
#pragma once

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/texture2d.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ew/external/stb_image.h"

using namespace cobb;
using namespace glm;


mat4 scale(const float x, const float y, const float z) {
    mat4 m = mat4(1);
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = z;
    return m;
}

mat4 rotate(const float yaw, const float pitch, const float roll) {
    const mat4 x = mat4(
        cos(yaw), 0, sin(yaw), 0,
        0, 1, 0, 0,
        -sin(yaw), 0, cos(yaw), 0,
        0, 0, 0, 1
    );
    const mat4 y = mat4(
        1, 0, 0, 0,
        0, cos(pitch), -sin(pitch), 0,
        0, sin(pitch), cos(pitch), 0,
        0, 0, 0, 1
    );
    const mat4 z = mat4(
        cos(roll), -sin(roll), 0, 0,
        sin(roll), cos(roll), 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );
    return y * x * z;
}

mat4 translate(const float x, const float y, const float z) {
    mat4 m = mat4(1);
    m[3][0] = x;
    m[3][1] = y;
    m[3][2] = z;
    return m;
}

mat4 lookAt(vec3 c, vec3 t, vec3 up) {
    vec3 f = normalize(t - c);
    vec3 r = normalize(cross(f, up));
    vec3 u = normalize(cross(r, f));
    mat4 m = mat4();
    return m;
}

static vec3 position;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {

    auto window = Window("THREEDEE");
    glfwSetKeyCallback(window.window, key_callback);
    constexpr float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);


    auto triangleShader = Shader("assets/assignment4/assignment4");
    triangleShader.use();


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    /*//Color
    glVertexAttribPointer(1, 4 , GL_FLOAT, GL_FALSE, 7 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);*/

    //mat4 temp = ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);

    
    glBindVertexArray(*Texture2d::getVAO());

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {

        if(glfwGetKey(window.window, GLFW_KEY_W)) {
            position.z-=0.01f;
        } else if(glfwGetKey(window.window, GLFW_KEY_S)) {
            position.z+=0.01f;
        } else if(glfwGetKey(window.window, GLFW_KEY_A)) {
            position.x-=0.01f;
        } else if(glfwGetKey(window.window, GLFW_KEY_D)) {
            position.x+=0.01f;
        } else if(glfwGetKey(window.window, GLFW_KEY_Q)) {
            position.y-=0.01f;
        } else if(glfwGetKey(window.window, GLFW_KEY_E)) {
            position.y+=0.01f;
        }
        //cout << position.x << " " << position.y << " " << position.z << endl;


        glfwPollEvents();
        window.update();
        //Drawing happens here!
        const auto time = static_cast<float>(glfwGetTime());

        triangleShader.setFloat("time", static_cast<float>(glfwGetTime()));
        triangleShader.setMat4("model", translate(position.x, position.y, position.z) * rotate(radians(-55.f), 0, 0) * scale(1, 1, 1));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
    glfwTerminate();
    return 0;
}