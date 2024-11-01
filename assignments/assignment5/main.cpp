//
// Created by andrew.genualdo on 9/17/2024.
//

#include <iostream>
#include <cstdio>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/objects/1d/line.hpp"
#include "cobb/objects/3d/cube.hpp"
#include "cobb/objects/3d/object.hpp"
#include "cobb/objects/3d/camera.hpp"
#include "cobb/objects/3d/lightSource.hpp"

using namespace cobb;
using namespace glm;

static Camera camera;

//taken directly from https://www.glfw.org/docs/3.3/input_guide.html
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    camera.handleKeyboard(key, action);
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) /*glfwSetWindowShouldClose(window, GLFW_TRUE);*/ {
            if(camera.lock) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    }
}

void mouse_position_callback(GLFWwindow* window, double xpos, double ypos) {
    camera.handleMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.handleScroll(yoffset);
}

int main() {

    auto window = Window("THREEDEE");
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init();


    auto shader = Shader("assets/assignment5/assignment5");
    shader.use();

    Cube bread = Cube("assets/assignment5/cube.png", GL_NEAREST);
    bread.bind();

    shader.use();
    shader.setInt("tex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(*Cube::getVAO());

    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));

    constexpr int randomBlockCount = 1024;
    Object randomBlocks[randomBlockCount];

    for(int i=0;i<randomBlockCount;i++) {
        bool repeat = true;
        while(repeat) {
            repeat = false;
            randomBlocks[i] = Object(
                    vec3(ew::RandomRange(0, 30) - 15, ew::RandomRange(0, 30) - 15, ew::RandomRange(0, 30) - 15),
                    vec3(0),
                    vec3(0.5f));
            randomBlocks[i]._position = normalize(randomBlocks[i]._position) * vec3(15);
            randomBlocks[i]._position = vec3((int) randomBlocks[i]._position.x, (int) randomBlocks[i]._position.y, (int) randomBlocks[i]._position.z);

            for(int j=0;j<i;j++) {
                if(randomBlocks[i]._position == randomBlocks[j]._position) {
                    repeat = true;
                    break;
                }
            }
        }


    }

    bool drawLightSource = true;
    bool moveLightSource = true;
    float moveSpeed = 0.5f;
    bool rainbowLight = true;
    float rainbowSaturation = 0.0f;
    float rainbowSpeed = 1.6f;
    vec3 ambientSpecularShininess = vec3(0.1f, 0.5f, 32.0f);
    bool moveLightToOrigin = false;

    LightSource lightSource = LightSource(vec3(0, 0, 0), vec4(0, 1, 1, 1));

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        float deltaTime = window.update();
        float time = window.getTime();

        //ImGui stuff
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Settings :D");
        //ImGui::Text("Yo theres text here now :D");
        ImGui::Text(camera.lock ? "Paused (ESC): true" : "Paused (ESC): false");
        ImGui::Checkbox("Draw Light", &drawLightSource);
        ImGui::Checkbox("Move Light", &moveLightSource);
        if(moveLightSource) {
            ImGui::SliderFloat("Light Speed", &moveSpeed, 0.1f, 10.0f);
        } else {
            ImGui::DragFloat3("Light Pos", &lightSource.pos.x);
            ImGui::Checkbox("Move Light to Origin", &moveLightToOrigin);
            if(moveLightToOrigin) {
                lightSource.pos = vec3(0);
                moveLightToOrigin = false;
            }
        }
        ImGui::Checkbox("Rainbow Light", &rainbowLight);
        if(rainbowLight) {
            ImGui::SliderFloat("Rainbow Speed", &rainbowSpeed, 0.1f, 10.0f);
            ImGui::SliderFloat("Saturation", &rainbowSaturation, -1.0f, 1.0f);
        } else {
            ImGui::ColorEdit4("Light Color", &lightSource.color.r);
        }
        ImGui::SliderFloat("Ambient K", &ambientSpecularShininess.x, 0.0f, 1.0f);
        ImGui::SliderFloat("Specular K", &ambientSpecularShininess.y, 0.0f, 1.0f);
        ImGui::SliderFloat("Shininess", &ambientSpecularShininess.z, 0.0f, 64.0f);
        ImGui::End();

        bread.bind();
        shader.use();
        glBindVertexArray(*Cube::getVAO());
        camera.update(window.window, deltaTime);
        shader.setMat4("view", camera.view);
        shader.setMat4("proj", camera.proj);
        shader.setVec3("cameraPos", camera._position);
        shader.setVec3("ambientSpecularShininess", ambientSpecularShininess);
        //shader.setFloat("shinyness", glm::pow(2, lightShinyness));
        shader.setVec3("light.pos", lightSource.pos);
        shader.setVec3("light.color", lightSource.color);

        for(int i=0;i<randomBlockCount;i++) {
            mat4 t = Object::translate(randomBlocks[i]._position.x, randomBlocks[i]._position.y, randomBlocks[i]._position.z);
            mat4 r = Object::rotate(radians(randomBlocks[i]._rotation.x), radians(randomBlocks[i]._rotation.y), radians(randomBlocks[i]._rotation.z));
            mat4 s = Object::scale(randomBlocks[i]._scale.x, randomBlocks[i]._scale.y, randomBlocks[i]._scale.z);
            mat4 model = t * r * s;
            shader.setMat4("model", model);
            shader.setMat3("transposeInverseModel", mat3(transpose(inverse(model))));
            bread.draw();
        }


        if(moveLightSource) {
            lightSource.pos.x = cos(time * moveSpeed * 1.1f) * 15;
            lightSource.pos.y = sin(time * moveSpeed * 1.2f) * 15;
            lightSource.pos.z = sin(time * moveSpeed * 1.3f) * 15;
        }

        if(rainbowLight) {
            lightSource.color.r = glm::max(glm::min((sin(rainbowSpeed * time) + 1.0f) * 0.5f + rainbowSaturation, 1.0f), 0.0f);
            lightSource.color.g = glm::max(glm::min((sin((rainbowSpeed * time) - glm::two_pi<float>() / 3.0f) + 1.0f) * 0.5f + rainbowSaturation, 1.0f), 0.0f);
            lightSource.color.b = glm::max(glm::min((sin((rainbowSpeed * time) - glm::two_pi<float>() * 2.0f / 3.0f) + 1.0f) * 0.5f + rainbowSaturation, 1.0f), 0.0f);
        }

        //light source
        if(drawLightSource) {
            lightShader->use();
            lightShader->setMat4("view", camera.view);
            lightShader->setMat4("proj", camera.proj);
            lightShader->setVec4("color", lightSource.color);
            lightShader->setMat4("model", Object::translate(lightSource.pos.x, lightSource.pos.y, lightSource.pos.z) * Object::scale(0.5f, 0.5f, 0.5f));
            lightSource.draw();
        }


        //axis gizmo
        if(camera.ui) {
            glDisable(GL_DEPTH_TEST);

            //higher number = less stuttering, can't be above ~950 because depth will start cutting it off, 900 to be safe
            constexpr float compassScale = 900.0f;

            vec3 p = camera.forward * vec3(compassScale) + camera._position;

            Line xAxis = Line(p, p + vec3(0.05f * compassScale, 0, 0), vec4(1, 0, 0, 1));
            Line yAxis = Line(p, p + vec3(0, 0.05f * compassScale, 0), vec4(0, 1, 0, 1)); //up
            Line zAxis = Line(p, p + vec3(0, 0, 0.05f * compassScale), vec4(0, 0, 1, 1));

            lineShader->use();
            lineShader->setMat4("viewProj", camera.proj * camera.view);
            glBindVertexArray(*Line::getVAO());

            xAxis.draw(&camera);
            yAxis.draw(&camera);
            zAxis.draw(&camera);


            glEnable(GL_DEPTH_TEST);
        }



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window.window);
    }
    cout << "Shutting Down..." << endl;
    glfwTerminate();
    return 0;
}

