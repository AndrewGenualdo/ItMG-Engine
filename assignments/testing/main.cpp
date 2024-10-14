//
// Created by andrew.genualdo on 9/17/2024.
//

#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/objects/3d/camera.hpp"
#include "cobb/objects/3d/cube.hpp"
#include "cobb/objects/1d/line.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>


using namespace cobb;
using namespace glm;


static Camera camera;

const static int WORLD_LENGTH = 64;
const static int WORLD_SIZE = WORLD_LENGTH * WORLD_LENGTH * WORLD_LENGTH;
static Object blocks[WORLD_SIZE];
static int wireframeMode = 0;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    camera.handleKeyboard(key, action);
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            if (camera.lock)
            {
                glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

            } else
            {
                glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
    } else if(key == GLFW_KEY_F3) {
        wireframeMode++;
        if(wireframeMode > 2) {
            wireframeMode = 0;
        }
    }
}

void mouse_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    camera.handleMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.handleScroll(yoffset);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {}
}

class WireCube {
public:
    Line *wires;

    WireCube(vec3 pos, vec4 color) {
        wires = new Line[12];

        wires[0]  = Line(vec3(0, 0, 0) + pos, vec3(1, 0, 0) + pos, 1, color);
        wires[1]  = Line(vec3(0, 0, 0) + pos, vec3(0, 1, 0) + pos, 1, color);
        wires[2]  = Line(vec3(0, 0, 0) + pos, vec3(0, 0, 1) + pos, 1, color);
        wires[3]  = Line(vec3(1, 1, 1) + pos, vec3(1, 1, 0) + pos, 1, color);
        wires[4]  = Line(vec3(1, 1, 1) + pos, vec3(0, 1, 1) + pos, 1, color);
        wires[5]  = Line(vec3(1, 1, 1) + pos, vec3(1, 0, 1) + pos, 1, color);
        wires[6]  = Line(vec3(1, 0, 0) + pos, vec3(1, 1, 0) + pos, 1, color);
        wires[7]  = Line(vec3(0, 1, 0) + pos, vec3(1, 1, 0) + pos, 1, color);
        wires[8]  = Line(vec3(0, 1, 0) + pos, vec3(0, 1, 1) + pos, 1, color);
        wires[9]  = Line(vec3(0, 0, 1) + pos, vec3(0, 1, 1) + pos, 1, color);
        wires[10] = Line(vec3(0, 0, 1) + pos, vec3(1, 0, 1) + pos, 1, color);
        wires[11] = Line(vec3(1, 0, 0) + pos, vec3(1, 0, 1) + pos, 1, color);
    }

    ~WireCube() {
        delete [] wires;
    }

    void draw(Camera *cam) {
        for(int i=0;i<12;i++) {
            wires[i].draw(cam);
        }
    }
};

struct Plane {
    Line *wires;

    Plane(vec3 pos, vec3 pos2, vec4 color) {
        wires = new Line[6];
        wires[0] = Line(pos, pos2, 1, color);
        if(pos.x == pos2.x) {
            wires[1] = Line(vec3(pos.x, pos2.y, pos.z), vec3(pos2.x, pos.y, pos2.z), 1, color);
        } else if(pos.y == pos2.y) {
            wires[1] = Line(vec3(pos2.x, pos.y, pos.z), vec3(pos.x, pos2.y, pos2.z), 1, color);
        } else {
            wires[1] = Line(vec3(pos.x, pos.y, pos2.z), vec3(pos2.x, pos2.y, pos.z), 1, color);
        }

        wires[2] = Line(wires[0]._start, wires[1]._start, 1, color);
        wires[3] = Line(wires[0]._start, wires[1]._end, 1, color);
        wires[4] = Line(wires[0]._end, wires[1]._start, 1, color);
        wires[5] = Line(wires[0]._end, wires[1]._end, 1, color);
    }

    ~Plane() {
        delete [] wires;
    }

    void draw(vec3 *pos, Camera *cam) {
        for(int i=1;i<6;i++) {
            wires[i].draw(pos, cam);
        }
    }


};

struct Coord {
    int x, y, z;

    Coord(int X, int Y, int Z) {
        x = X;
        y = Y;
        z = Z;
    }

};

void drawPlaneWithCheck(Plane *planes, int plane, vec3 *pos, Camera *cam) {
    switch(plane) {
        case 0: {
            if(pos->z - 1 >= 0) {
                int zPos = pos->y * WORLD_LENGTH * WORLD_LENGTH + (pos->z - 1) * WORLD_LENGTH + pos->x;
                if(!blocks[zPos].exists) {
                    planes[plane].draw(pos, cam);
                }
            }
            return;
        }
        case 1: {
            if(pos->x - 1 >= 0) {
                int xPos = pos->y * WORLD_LENGTH * WORLD_LENGTH + pos->z * WORLD_LENGTH + (pos->x - 1);
                if(!blocks[xPos].exists) {
                    planes[plane].draw(pos, cam);
                }
            }
            return;
        }
        case 2: {
            if(pos->y - 1 >= 0) {
                int yPos = (pos->y - 1) * WORLD_LENGTH * WORLD_LENGTH + pos->z * WORLD_LENGTH + pos->x;
                if(!blocks[yPos].exists) {
                    planes[plane].draw(pos, cam);
                }
            }
            return;
        }
        case 3: {
            if(pos->x + 1 < WORLD_LENGTH) {
                int xPos = pos->y * WORLD_LENGTH * WORLD_LENGTH + pos->z * WORLD_LENGTH + (pos->x + 1);
                if(!blocks[xPos].exists) {
                    planes[plane].draw(pos, cam);
                }
            }
            return;
        }
        case 4: {
            if(pos->y + 1 < WORLD_LENGTH) {
                int yPos = (pos->y + 1) * WORLD_LENGTH * WORLD_LENGTH + pos->z * WORLD_LENGTH + pos->x;
                if(!blocks[yPos].exists) {
                    planes[plane].draw(pos, cam);
                }
            }
            return;
        }
        case 5: {
            if(pos->z + 1 < WORLD_LENGTH) {
                int zPos = pos->y * WORLD_LENGTH * WORLD_LENGTH + (pos->z + 1) * WORLD_LENGTH + pos->x;
                if(!blocks[zPos].exists) {
                    planes[plane].draw(pos, cam);
                }
            }
            return;
        }


    }
}

int main()
{

    auto window = Window("Minecraft?!!!");
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSetMouseButtonCallback(window.window, mouse_button_callback);
    glfwSwapInterval(0);

    auto cubeShader = Shader("assets/assignment4/assignment4");
    cubeShader.use();

    //this is a picture of my friend from 5th grade (ish) and I though it was funny (don't worry, I asked him)
    Cube block = Cube("assets/testing/grass.png", GL_NEAREST);
    block.bind();

    cubeShader.use();
    cubeShader.setInt("tex", 0);

    cubeShader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(*Cube::getVAO());

    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));

    for (int y = 0; y < WORLD_LENGTH; y++) {
        for (int z = 0; z < WORLD_LENGTH; z++) {
            for (int x = 0; x < WORLD_LENGTH; x++) {
                int arrayPos = y * WORLD_LENGTH * WORLD_LENGTH + z * WORLD_LENGTH + x;
                blocks[arrayPos] = Object(vec3(x, y , z), vec3(0), vec3(1));
                if(y > 0 && y > 1 + sin(distance(vec2(x, z), vec2(WORLD_LENGTH/2)) / 3) / 3 + sin(z * 2) / 2 + cos(x * 2) / 2 + sin(x + z) / 2 + cos(x + z) / 2) {
                    blocks[arrayPos].exists = false;
                }

            }
        }
    }

    Plane planes[6] = {
            Plane(vec3(0), vec3(1, 1, 0), vec4(1, 0, 0, 1)),
            Plane(vec3(0), vec3(0, 1, 1), vec4(0, 1, 0, 1)),
            Plane(vec3(0), vec3(1, 0, 1), vec4(0, 0, 1, 1)),
            Plane(vec3(1), vec3(1, 0, 0), vec4(1, 1, 0, 1)),
            Plane(vec3(1), vec3(0, 1, 0), vec4(0, 1, 1, 1)),
            Plane(vec3(1), vec3(0, 0, 1), vec4(1, 0, 1, 1))
    };

    Coord cachedPlanes[8] = {
            Coord(0, 1, 2),
            Coord(0, 2, 3),
            Coord(0, 1, 4),
            Coord(0, 3, 4),
            Coord(1, 2, 5),
            Coord(2, 3, 5),
            Coord(1, 4, 5),
            Coord(3, 4, 5)
    };


    //settings
    int steps = 50; //how many steps to check which block is being looked at (accuracy basically)
    int dist = 10; //how far to search for a block to be looked at
    float wireframeRadius = 12.5f; //how far (along xz plane) to draw the wireframe when using wireframeMode (F3)

    //"global" variables
    int lookingAt = -1; //block being looked at
    bool wasLeftPressed = false;
    bool wasRightPressed = false;


    //Render loop
    while (!glfwWindowShouldClose(window.window))
    {
        glfwPollEvents();

        bool activeLeft = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_LEFT);
        bool activeRight = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_RIGHT);
        bool isLeftPressed = activeLeft && !wasLeftPressed;
        bool isRightPressed = activeRight && !wasRightPressed;
        wasLeftPressed = activeLeft;
        wasRightPressed = activeRight;



        float deltaTime = window.update();
        float time = window.getTime();

        camera.update(window.window, deltaTime);
        lineShader->use();
        lineShader->setMat4("viewProj", camera.proj * camera.view);
        glBindVertexArray(*Line::getVAO());

        if(lineShader != nullptr && wireframeMode > 0) {
            for(int i=0;i<WORLD_SIZE;i++) {
                if(!blocks[i].exists || length(vec2(blocks[i]._position.x, blocks[i]._position.z) - vec2(camera._position.x, camera._position.z)) > wireframeRadius) continue;
                //Coord is just a vec3 but ints instead of floats
                vec3 cubeCenter = blocks[i]._position + vec3(0.5f);
                Coord coord = cachedPlanes[(camera._position.x > cubeCenter.x ? 1 : 0) + (camera._position.y > cubeCenter.y ? 2 : 0) + (camera._position.z > cubeCenter.z ? 4 : 0)];
                drawPlaneWithCheck(planes, coord.x, &blocks[i]._position, &camera);
                drawPlaneWithCheck(planes, coord.y, &blocks[i]._position, &camera);
                drawPlaneWithCheck(planes, coord.z, &blocks[i]._position, &camera);
            }
        }





        //Draw all cubes here!
        cubeShader.use();
        cubeShader.setMat4("viewProj", camera.proj * camera.view);
        cubeShader.setBool("lock", camera.lock);
        block.bind();
        glBindVertexArray(*Cube::getVAO());
        for(int i=0;i<WORLD_SIZE;i++) {
            if(!blocks[i].exists) continue;
            if(wireframeMode == 2 && length(vec2(blocks[i]._position.x, blocks[i]._position.z) - vec2(camera._position.x, camera._position.z)) <= wireframeRadius) continue;
            cubeShader.setBool("hover", lookingAt == i);
            cubeShader.setMat4("model", Object::translate(blocks[i]._position.x, blocks[i]._position.y, blocks[i]._position.z));
            block.draw();
        }



        if(camera.lock)
        {
            glfwSwapBuffers(window.window);
            continue;
        }
        for(int i=0;i<steps;i++) {
            vec3 forward = camera._position + (camera.cameraFront * vec3(static_cast<float>(i) / static_cast<float>(steps / dist)));
            if(forward.x >= 0 && forward.y >= 0 && forward.z >= 0) {
                int arrayPos = floor(forward.y) * WORLD_LENGTH * WORLD_LENGTH + floor(forward.z) * WORLD_LENGTH + floor(forward.x);
                if(arrayPos >= 0 && arrayPos < WORLD_SIZE) {
                    lookingAt = arrayPos;
                    if(blocks[arrayPos].exists) {
                        if(isLeftPressed) blocks[lookingAt].exists = false;
                        break;
                    }
                }
            }
        }

        if(isRightPressed && lookingAt != -1) {
            int newSpot = lookingAt + WORLD_LENGTH * WORLD_LENGTH; //+1 on the y
            if(newSpot >= 0 && newSpot < WORLD_SIZE) {
                blocks[newSpot].exists = true;
            }
        }

        if(camera.ui && lineShader != nullptr) {
            glDisable(GL_DEPTH_TEST);

            //higher number = less stuttering, can't be above ~950 because depth will start cutting it off, 900 to be safe
            const float compassScale = 900.0f;

            vec3 p1 = camera.cameraFront * vec3(compassScale) + camera._position;
            vec3 p2 = camera.cameraFront * vec3(compassScale) + camera._position;

            Line xAxis = Line(p1, p2 + vec3(0.05f * compassScale, 0, 0), 1, vec4(1, 0, 0, 1));
            Line yAxis = Line(p1, p2 + vec3(0, 0.05f * compassScale, 0), 1, vec4(0, 1, 0, 1)); //up
            Line zAxis = Line(p1, p2 + vec3(0, 0, 0.05f * compassScale), 1, vec4(0, 0, 1, 1));
            xAxis.draw(&camera, true);
            yAxis.draw(&camera, false);
            zAxis.draw(&camera, false);

            glEnable(GL_DEPTH_TEST);
        }



        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
    glfwTerminate();
    return 0;
}