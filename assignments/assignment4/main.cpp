//
// Created by andrew.genualdo on 9/17/2024.
//

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/objects/3d/cube.hpp"
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

//position = Camera position
//rotation = Camera direction
//scale = Camera zoom?

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
        blockList[line * 10 + i].rotation = vec3(0);
        blockList[line * 10 + i].scale = vec3(1);
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
                camera.lock = false;
                glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                pauseDuration+= glfwGetTime() - pauseStart;
            } else {
                camera.lock = true;
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                pauseStart = glfwGetTime();
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

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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
    glfwSetCursorPosCallback(window.window, mouse_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSwapInterval(1);

    auto shader = Shader("assets/assignment4/assignment4");
    shader.use();

    //this is a picture of my friend from 5th grade and I though it was funny (don't worry, I asked him)
    Cube bread = Cube("assets/assignment4/owen.jpg", GL_NEAREST);
    bread.bind();
    //glBindVertexArray(*Texture2d::getVAO());


    shader.use();
    shader.setInt("tex", 0);

    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(*Cube::getVAO());

    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 45.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));

    for(int i=0; i < BLOCK_COUNT; i++) {
        blockList[i] = Object();
        vec2 world = toWorld(i);
        blockList[i].position = vec3(world.x - 5.0f, world.y, 0);
    }

    const int randomBlockCount = 100;
    Object randomBlocks[randomBlockCount];

    for(int i=0;i<randomBlockCount;i++) {
        randomBlocks[i] = Object(vec3(ew::RandomRange(0, 30) - 15, ew::RandomRange(0, 30) - 15, 50 + ew::RandomRange(0, 30)), vec3(0), vec3(1));
    }

    float lastTick = 0.0f;

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        float deltaTime = window.update();
        float time = window.getTime();
        glfwPollEvents();


        const float tickLength = static_cast<float>(pow(0.95f * pow(0.99f, static_cast<int>(time - lastRestart - pauseDuration)), lineClears)) * 0.5f;
        if(time > lastTick + tickLength) {
            lastTick+=tickLength;
            tick();
        }

        bread.bind();
        shader.setBool("gameOver", activeBlock == -3);
        shader.setBool("lock", camera.lock);

        camera.update(window.window, deltaTime);
        shader.setMat4("view", camera.view);
        shader.setMat4("proj", camera.proj);

        for(int i=0; i < BLOCK_COUNT; i++) {
            
            if(!blockList[i].exists) {
                continue;
            }

            mat4 t = translate(blockList[i].position.x, blockList[i].position.y, blockList[i].position.z);
            mat4 r = rotate(radians(blockList[i].rotation.x), radians(blockList[i].rotation.y), radians(blockList[i].rotation.z));
            mat4 s = scale(blockList[i].scale.x, blockList[i].scale.y, blockList[i].scale.z);

            if(activeBlock == -2) {
                float clearTime = time - clearStart;
                if(i / 10 == clearLineId) {
                    if(clearTime < 1.5f) {
                        blockList[i].rotation.z = mod(time * 1000, 360.0f);
                        blockList[i].rotation.x = mod(time * 1000, 360.0f);
                        if(clearTime >= 1.0f) {
                            float scale = (0.5f - (clearTime - 1.0f)) * 2.5f;
                            blockList[i].scale = vec3(scale, scale, scale);
                        }
                    } else if(clearTime >= 1.5f) {
                        clearLine(clearLineId);
                    }
                } else if(clearTime > 1.0f) {
                    float shift = (clearTime - 1.0f) * 2.0f;
                    t = translate(blockList[i].position.x, blockList[i].position.y - shift, blockList[i].position.z);
                }
            }
            //t * r * s = model matrix
            shader.setMat4("model", t * r * s);
            bread.draw();

        }


        for(int i=0;i<randomBlockCount;i++) {

            randomBlocks[i].position.x = clamp(randomBlocks[i].position.x + ew::RandomRange(-0.1f, 0.1f), -15.0f, 15.0f);
            randomBlocks[i].position.y = clamp(randomBlocks[i].position.y + ew::RandomRange(-0.1f, 0.1f), -15.0f, 15.0f);
            randomBlocks[i].position.z = clamp(randomBlocks[i].position.z + ew::RandomRange(-0.1f, 0.1f), 50.0f, 80.0f);

            randomBlocks[i].scale.x = clamp(randomBlocks[i].scale.x + ew::RandomRange(-0.1f, 0.1f), 0.1f, 5.0f);
            randomBlocks[i].scale.y = clamp(randomBlocks[i].scale.y + ew::RandomRange(-0.1f, 0.1f), 0.1f, 5.0f);
            randomBlocks[i].scale.z = clamp(randomBlocks[i].scale.z + ew::RandomRange(-0.1f, 0.1f), 0.1f, 5.0f);

            randomBlocks[i].rotation.x += ew::RandomRange(-3.0f, 3.0f);
            randomBlocks[i].rotation.y += ew::RandomRange(-3.0f, 3.0f);
            randomBlocks[i].rotation.z += ew::RandomRange(-3.0f, 3.0f);

            mat4 t = translate(randomBlocks[i].position.x, randomBlocks[i].position.y, randomBlocks[i].position.z);
            mat4 r = rotate(radians(randomBlocks[i].rotation.x), radians(randomBlocks[i].rotation.y), radians(randomBlocks[i].rotation.z));
            mat4 s = scale(randomBlocks[i].scale.x, randomBlocks[i].scale.y, randomBlocks[i].scale.z);
            shader.setMat4("model", t * r * s);
            bread.draw();
        }


        glfwSwapBuffers(window.window);
    }
    cout << "Shutting Down..." << endl;
    glfwTerminate();
    return 0;
}

