//
// Created by andrew.genualdo on 9/17/2024.
//

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/objects/3d/cube.hpp"
#include "cobb/objects/1d/line.hpp"
#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ew/external/stb_image.h"
#include "cobb/objects/3d/object.hpp"
#include "cobb/objects/3d/camera.hpp"

using namespace cobb;
using namespace glm;

static Camera camera;

static int activeBlock = -1;
const static int BLOCK_COUNT = 200;
static Object blockList[BLOCK_COUNT] = {};
static int clearLineId = -1;
static float clearStart = 0.0f;
static int lineClears = 0;
static float lastRestart = 0.0f;
static float pauseStart = 0.0f;
static float pauseDuration = 0.0f;

static int toId(vec2 world) {
    return static_cast<int>(world.y * 10 + world.x);
}

static vec2 toWorld(int id) {
    return vec2(id%10, id/10);
}

void setActiveBlock(int id) {
    if(!blockList[id].exists) {
        blockList[activeBlock].exists = false;
        activeBlock = id;
        blockList[activeBlock].exists = true;
    }
}

void settle() {
    vec2 world = toWorld(activeBlock);
    if(static_cast<int>(world.y) == BLOCK_COUNT / 10 - 1) {
        activeBlock = -3; //game over
        cout << "Game Over! Score: " << lineClears << endl;
        cout << "Press 'R' to restart!" << endl;
        return;
    }
    activeBlock = -1;
    for(int i=0;i<10;i++) {
        if(!blockList[toId(vec2(i, world.y))].exists) {
            return;
        }
    }
    activeBlock = -2;
    clearLineId = static_cast<int>(world.y);
    clearStart = static_cast<float>(glfwGetTime());
}

void clearLine(int line) {
    lineClears++;
    for(int i=0;i<10;i++) {
        blockList[line * 10 + i]._rotation = vec3(0);
        blockList[line * 10 + i]._scale = vec3(0.5f);
        blockList[line * 10 + i].exists = false;
    }
    for(int i=10;i<BLOCK_COUNT;i++) {
        if(blockList[i].exists) {
            blockList[i-10].exists = true;
            blockList[i].exists = false;
        }
    }
    activeBlock = -1;
}

void tick() {
    //camera.lock = paused, -3 = game over, -2 = line clear animation in progress
    if(camera.lock || activeBlock == -3 || activeBlock == -2) {
        return;
    } else if(activeBlock == -1) {
        activeBlock = 195;
        blockList[activeBlock].exists = true;
    } else if(activeBlock < 10 || blockList[activeBlock - 10].exists) {
        settle();
    } else {
        setActiveBlock(activeBlock-10);
    }
}

void restartGame() {
    for(int i=0;i<BLOCK_COUNT;i++) {
        blockList[i].exists = false;
    }
    lastRestart = static_cast<float>(glfwGetTime());
    pauseDuration = 0.0f;
    lineClears = 0;
    activeBlock = -1;
    camera.reset();
}

//taken directly from https://www.glfw.org/docs/3.3/input_guide.html
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    camera.handleKeyboard(key, action);
    if(action == GLFW_PRESS) {
        if(key == GLFW_KEY_ESCAPE) /*glfwSetWindowShouldClose(window, GLFW_TRUE);*/ {
            if(camera.lock) {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                pauseStart = glfwGetTime();
            } else {
                glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                pauseDuration+= glfwGetTime() - pauseStart;
            }
        }

        if(key == GLFW_KEY_R) restartGame();
        if(activeBlock >= 0) {
            if(key == GLFW_KEY_RIGHT) setActiveBlock(activeBlock+1);
            if(key == GLFW_KEY_LEFT) setActiveBlock(activeBlock-1);
            if(key == GLFW_KEY_DOWN) {
                while(activeBlock >= 10) {
                    if(blockList[activeBlock-10].exists) {
                        settle();
                        return;
                    }
                    setActiveBlock(activeBlock-10);
                }
                settle();
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

    auto shader = Shader("assets/assignment4/assignment4");
    shader.use();

    //this is a picture of my friend from 5th grade (ish) and I though it was funny (don't worry, I asked him)
    Cube bread = Cube("assets/assignment4/owen.jpg", GL_NEAREST);
    bread.bind();

    shader.use();
    shader.setInt("tex", 0);

    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(*Cube::getVAO());

    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));

    for(int i=0; i < BLOCK_COUNT; i++) {
        blockList[i] = Object();
        vec2 world = toWorld(i);
        blockList[i]._position = vec3(world.x - 5.0f, world.y, 0);
        blockList[i]._scale = vec3(0.5f);
    }

    const int randomBlockCount = 25;
    Object randomBlocks[randomBlockCount];

    for(int i=0;i<randomBlockCount;i++) {
        randomBlocks[i] = Object(vec3(ew::RandomRange(0, 30) - 15, ew::RandomRange(0, 30) - 15, 50 + ew::RandomRange(0, 30)), vec3(ew::RandomRange(0, 360), ew::RandomRange(0, 360), ew::RandomRange(0, 360)), vec3(ew::RandomRange(0.1f, 5.0f), ew::RandomRange(0.1f, 5.0f), ew::RandomRange(0.1f, 5.0f)));
    }

    float lastTick = 0.0f;

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        float deltaTime = window.update();
        float time = window.getTime();


        const float tickLength = static_cast<float>(pow(0.95f * pow(0.99f, static_cast<int>(time - lastRestart - pauseDuration)), lineClears)) * 0.5f;
        if(time > lastTick + tickLength) {
            lastTick+=tickLength;
            tick();
        }

        bread.bind();
        shader.use();
        glBindVertexArray(*Cube::getVAO());
        shader.setBool("gameOver", activeBlock == -3);
        shader.setBool("lock", camera.lock);

        camera.update(window.window, deltaTime);
        shader.setMat4("view", camera.view);
        shader.setMat4("proj", camera.proj);

        for(int i=0; i < BLOCK_COUNT; i++) {
            
            if(!blockList[i].exists) {
                continue;
            }

            mat4 t = Object::translate(blockList[i]._position.x, blockList[i]._position.y, blockList[i]._position.z);
            mat4 r = Object::rotate(radians(blockList[i]._rotation.x), radians(blockList[i]._rotation.y), radians(blockList[i]._rotation.z));
            mat4 s = Object::scale(blockList[i]._scale.x, blockList[i]._scale.y, blockList[i]._scale.z);

            if(activeBlock == -2) {
                float clearTime = time - clearStart;
                if(i / 10 == clearLineId) {
                    if(clearTime < 1.5f) {
                        blockList[i]._rotation.z = mod(time * 1000, 360.0f);
                        blockList[i]._rotation.x = mod(time * 1000, 360.0f);
                        if(clearTime >= 1.0f) {
                            float scale = (0.5f - (clearTime - 1.0f)) * 2.5f;
                            blockList[i]._scale = vec3(scale * 0.5f, scale * 0.5f, scale * 0.5f);
                        }
                    } else if(clearTime >= 1.5f) {
                        clearLine(clearLineId);
                    }
                } else if(clearTime > 1.0f) {
                    float shift = (clearTime - 1.0f) * 2.0f;
                    t = Object::translate(blockList[i]._position.x, blockList[i]._position.y - shift, blockList[i]._position.z);
                }
            }
            //t * r * s = model matrix
            shader.setMat4("model", t * r * s);
            bread.draw();

        }


        for(int i=0;i<randomBlockCount;i++) {
            if(!camera.lock) {
                randomBlocks[i]._position.x = clamp(randomBlocks[i]._position.x + ew::RandomRange(-10.0f, 10.0f) * deltaTime, -15.0f, 15.0f);
                randomBlocks[i]._position.y = clamp(randomBlocks[i]._position.y + ew::RandomRange(-10.0f, 10.0f) * deltaTime, -15.0f, 15.0f);
                randomBlocks[i]._position.z = clamp(randomBlocks[i]._position.z + ew::RandomRange(-10.0f, 10.0f) * deltaTime, 50.0f, 80.0f);

                randomBlocks[i]._scale.x = clamp(randomBlocks[i]._scale.x + ew::RandomRange(-100.0f, 100.0f) * deltaTime, 0.1f, 5.0f);
                randomBlocks[i]._scale.y = clamp(randomBlocks[i]._scale.y + ew::RandomRange(-100.0f, 100.0f) * deltaTime, 0.1f, 5.0f);
                randomBlocks[i]._scale.z = clamp(randomBlocks[i]._scale.z + ew::RandomRange(-100.0f, 100.0f) * deltaTime, 0.1f, 5.0f);

                randomBlocks[i]._rotation.x += ew::RandomRange(-1000.0f, 1000.0f) * deltaTime;
                randomBlocks[i]._rotation.y += ew::RandomRange(-1000.0f, 1000.0f) * deltaTime;
                randomBlocks[i]._rotation.z += ew::RandomRange(-1000.0f, 1000.0f) * deltaTime;
            }

            mat4 t = Object::translate(randomBlocks[i]._position.x, randomBlocks[i]._position.y, randomBlocks[i]._position.z);
            mat4 r = Object::rotate(radians(randomBlocks[i]._rotation.x), radians(randomBlocks[i]._rotation.y), radians(randomBlocks[i]._rotation.z));
            mat4 s = Object::scale(randomBlocks[i]._scale.x, randomBlocks[i]._scale.y, randomBlocks[i]._scale.z);
            shader.setMat4("model", t * r * s);
            bread.draw();
        }

        if(camera.ui) {
            glDisable(GL_DEPTH_TEST);

            //higher number = less stuttering, can't be above ~950 because depth will start cutting it off, 900 to be safe
            const float compassScale = 900.0f;

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


        glfwSwapBuffers(window.window);
    }
    cout << "Shutting Down..." << endl;
    glfwTerminate();
    return 0;
}

