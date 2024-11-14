//
// Created by andrew.genualdo on 9/17/2024.
//

#include <iomanip>
#include <iostream>
#include <cstdio>

#include "cobb/shader.hpp"
#include "cobb/window.hpp"
#include "cobb/objects/3d/camera.hpp"
#include "cobb/objects/3d/cube.hpp"
#include "cobb/objects/1d/line.hpp"
#include "cobb/objects/1d/fadeLine.hpp"
#include "ew/ewMath/ewMath.h"
#include "cobb/objects/2d/triangle.hpp"
#include "cobb/objects/2d/fadeTriangle.hpp"
#include "cobb/objects/3d/geometry.hpp"
#include <ew/external/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


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
        } else if(key == GLFW_KEY_F3) {
            wireframeMode++;
            if(wireframeMode > 2) {
                wireframeMode = 0;
            }
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

class WireCube {
public:
    Line *wires;

    WireCube(vec3 pos, vec4 color) {
        wires = new Line[12];

        wires[0]  = Line(vec3(0, 0, 0) + pos, vec3(1, 0, 0) + pos, color);
        wires[1]  = Line(vec3(0, 0, 0) + pos, vec3(0, 1, 0) + pos, color);
        wires[2]  = Line(vec3(0, 0, 0) + pos, vec3(0, 0, 1) + pos, color);
        wires[3]  = Line(vec3(1, 1, 1) + pos, vec3(1, 1, 0) + pos, color);
        wires[4]  = Line(vec3(1, 1, 1) + pos, vec3(0, 1, 1) + pos, color);
        wires[5]  = Line(vec3(1, 1, 1) + pos, vec3(1, 0, 1) + pos, color);
        wires[6]  = Line(vec3(1, 0, 0) + pos, vec3(1, 1, 0) + pos, color);
        wires[7]  = Line(vec3(0, 1, 0) + pos, vec3(1, 1, 0) + pos, color);
        wires[8]  = Line(vec3(0, 1, 0) + pos, vec3(0, 1, 1) + pos, color);
        wires[9]  = Line(vec3(0, 0, 1) + pos, vec3(0, 1, 1) + pos, color);
        wires[10] = Line(vec3(0, 0, 1) + pos, vec3(1, 0, 1) + pos, color);
        wires[11] = Line(vec3(1, 0, 0) + pos, vec3(1, 0, 1) + pos, color);
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
        wires[0] = Line(pos, pos2, color);
        if(pos.x == pos2.x) {
            wires[1] = Line(vec3(pos.x, pos2.y, pos.z), vec3(pos2.x, pos.y, pos2.z), color);
        } else if(pos.y == pos2.y) {
            wires[1] = Line(vec3(pos2.x, pos.y, pos.z), vec3(pos.x, pos2.y, pos2.z), color);
        } else {
            wires[1] = Line(vec3(pos.x, pos.y, pos2.z), vec3(pos2.x, pos2.y, pos.z), color);
        }

        wires[2] = Line(wires[0]._start, wires[1]._start, color);
        wires[3] = Line(wires[0]._start, wires[1]._end, color);
        wires[4] = Line(wires[0]._end, wires[1]._start, color);
        wires[5] = Line(wires[0]._end, wires[1]._end, color);
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

int world() {
    auto window = Window("Minecraft?!!!");
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSwapInterval(0);
    auto cubeShader = Shader("assets/assignment4/assignment4");
    cubeShader.use();
    cubeShader.setInt("tex", 0);


    Cube block = Cube("assets/testing/grass.png", GL_NEAREST);
    block.bind();

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



        if(wireframeMode > 0) {
            lineShader->use();
            lineShader->setMat4("viewProj", camera.proj * camera.view);
            glBindVertexArray(*Line::getVAO());
            for(int i=0;i<WORLD_SIZE;i++) {
                if(!blocks[i].exists) continue;
                if(length(cross(blocks[i]._position - camera._position + camera.forward, blocks[i]._position - camera._position)) / length(camera.forward) > wireframeRadius) continue;
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
            if(wireframeMode == 2 && length(cross(blocks[i]._position - camera._position + camera.forward, blocks[i]._position - camera._position)) / length(camera.forward) <= wireframeRadius) continue;
            cubeShader.setBool("hover", lookingAt == i);
            cubeShader.setMat4("model", Object::translate(blocks[i]._position.x + 0.5f, blocks[i]._position.y + 0.5f, blocks[i]._position.z + 0.5f) * Object::scale(0.5f, 0.5f, 0.5f));
            block.draw();
        }



        if(camera.lock)
        {
            glfwSwapBuffers(window.window);
            continue;
        }
        for(int i=0;i<steps;i++) {
            vec3 forward = camera._position + (camera.forward * vec3(static_cast<float>(i) / static_cast<float>(steps / dist)));
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
    cout << "Shutting down..." << endl;
    glfwTerminate();
    return 0;
}

struct MeshPoint {
    int x, z;
    float y;
    bool set;

    MeshPoint() {
        x = 0;
        y = 0.0f;
        z = 0;
        set = false;
    }

    MeshPoint(int _x, float _y, int _z, bool _set) {
        x = _x;
        y = _y;
        z = _z;
        set = _set;
    }
};

float getHeight(MeshPoint *positions, int length, int x, int z) {

    return positions[(z % length) * length + (x % length)].y;
}

float generateHeight(MeshPoint *positions, int length, int x, int z, float mult) {
    bool xPlusSet = positions[(x + 1)%length].set;
    bool xMinusSet = positions[(x + length - 1)%length].set;
    bool zPlusSet = positions[(z + 1)%length].set;
    bool zMinusSet = positions[(z + length - 1)%length].set;

    float xplus = xPlusSet ? getHeight(positions, length, x + 1, z) : 0.0f;
    float xminus = xMinusSet ? getHeight(positions, length, x - 1, z) : 0.0f;
    float zplus = zPlusSet ? getHeight(positions, length, x, z + 1) : 0.0f;
    float zminus = zMinusSet ? getHeight(positions, length, x, z - 1) : 0.0f;

    float strength = 0.5f;
    float func = 0.0f;//(sin(z) + cos(x) + ((x + z) / (2.0f * length)) - 0.25f) * 0.25f;
    //func+=(sin(x*1.3f + ew::RandomRange(0.0f, 100.0f)) + cos(z*1.6f + ew::RandomRange(0.0f, 100.0f)) > 0) ? mult * 4 : mult;
    //func*=mult;

    mult *= (sin(x * mult + ew::RandomRange(-100, 100)) + cos(z * mult + ew::RandomRange(-100, 100))) + 1.0f;

    if(xPlusSet) {
        if(zPlusSet) {
            return (xplus + zplus) * strength + ew::RandomRange(-mult, mult) + func;
        } else if(zMinusSet) {
            return (xplus + zminus) * strength + ew::RandomRange(-mult, mult) + func;
        } else {
            return xplus + ew::RandomRange(-mult, mult);
        }
    } else if(xMinusSet) {
        if(zPlusSet) {
            return (xminus + zplus) * strength + ew::RandomRange(-mult, mult) + func;
        } else if(zMinusSet) {
            return (xminus + zminus) * strength + ew::RandomRange(-mult, mult) + func;
        } else {
            return xminus + ew::RandomRange(-mult, mult);
        }
    } else if(zPlusSet) {
        if(xPlusSet) {
            return (zplus + xplus) * strength + ew::RandomRange(-mult, mult) + func;
        } else if(xMinusSet) {
            return (zplus + xminus) * strength + ew::RandomRange(-mult, mult) + func;
        } else {
            return zplus + ew::RandomRange(-mult, mult);
        }
    }  else if(zMinusSet) {
        if(xPlusSet) {
            return (zminus + xplus) * strength + ew::RandomRange(-mult, mult) + func;
        } else if(xMinusSet) {
            return (zminus + xminus) * strength + ew::RandomRange(-mult, mult) + func;
        } else {
            return zminus + ew::RandomRange(-mult, mult);
        }
    } else {
        return ew::RandomRange(-mult, mult);
    }
}



int mesh() {
    auto window = Window("terrain???");
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSwapInterval(0);
    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));
    Line::loadShader();
    FadeLine::loadShader();
    Triangle::loadShader();
    FadeTriangle::loadShader();
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    int length = 1;
    int max = 50;
    float drawDistance = 100.0f;
    float mult = 0.000001f;
    float highest = -999999999.0f;
    float lowest =   999999999.0f;
    MeshPoint *positions = new MeshPoint[max * max];



    int lineCount = max * (max - 1);
    FadeLine *xLines = new FadeLine[lineCount];
    FadeLine *zLines = new FadeLine[lineCount];

    int triangleCount = (max - 1) * (max - 1);
    auto *triangles = new FadeTriangle[triangleCount * 4];
    float *centerHeights = new float[triangleCount];

    for(int i=0;i<max*max;i++) {
        positions[i].set = false;
    }

    bool wasLeftPressed = false;
    bool wasRightPressed = false;

    float lastTick = 0.0f;

    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();

        bool activeLeft = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_LEFT);
        bool activeRight = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_RIGHT);
        bool isLeftPressed = activeLeft && !wasLeftPressed;
        bool isRightPressed = activeRight && !wasRightPressed;
        wasLeftPressed = activeLeft;
        wasRightPressed = activeRight;

        float deltaTime = window.update();
        float time = window.getTime();

        if(time > lastTick + 0.1f) {
            lastTick+=0.1f;
            if(!camera.lock) {
                highest = -999999999.0f;
            lowest =   999999999.0f;
            for(int i=0;i<max*max;i++) {
                positions[i].set = false;
            }

            for(int i=0;i<max * max;i++) {
                int x = i % max;
                int z = i / max;
                float y = generateHeight(positions, max, x, z, mult);
                positions[i].x = x;
                positions[i].y = y;
                positions[i].z = z;
                positions[i].set = true;
                highest = glm::max(y, highest);
                lowest = glm::min(y, lowest);

                if(wireframeMode == 0 && x > 0 && z > 0) {
                    float xmzm = positions[(z - 1) * max + (x - 1)].y;
                    float xpzm = positions[(z - 1) * max + x].y;
                    float xmzp = positions[z * max + (x - 1)].y;
                    float xpzp = positions[z * max + x].y;

                    centerHeights[(z - 1) * (max - 1) + (x - 1)] = (xmzm + xpzm + xmzp + xpzp) * 0.25f;
                }

            }

            if(wireframeMode == 0) {
                for(int z = 0; z < max - 1; z++) {
                    for(int x = 0; x < max - 1; x++) {
                        int arrayPos = z * (max - 1) + x;
                        int t0ArrayPos = 4 * arrayPos + 0;
                        int t1ArrayPos = 4 * arrayPos + 1;
                        int t2ArrayPos = 4 * arrayPos + 2;
                        int t3ArrayPos = 4 * arrayPos + 3;

                        arrayPos = z * max + x;

                        float highest0 = positions[arrayPos].y;//glm::max(glm::max(positions[arrayPos].y, positions[arrayPos + 1].y), centerHeights[z * (max - 1) + x]);
                        float highest1 = positions[arrayPos + 1].y;//glm::max(glm::max(positions[arrayPos + 1].y, positions[arrayPos + max + 1].y), centerHeights[z * (max - 1) + x]);
                        float highest2 = positions[arrayPos + max + 1].y;//glm::max(glm::max(positions[arrayPos + max + 1].y, positions[arrayPos + max].y), centerHeights[z * (max - 1) + x]);
                        float highest3 = positions[arrayPos + max].y;//glm::max(glm::max(positions[arrayPos + max].y, positions[arrayPos].y), centerHeights[z * (max - 1) + x]);

                        triangles[t0ArrayPos].p1 = vec3(x, positions[arrayPos].y, z); //possible
                        triangles[t0ArrayPos].p2 = vec3(x + 1, positions[arrayPos + 1].y, z); //possible
                        triangles[t0ArrayPos].p3 = vec3(x + 0.5f, centerHeights[z * (max - 1) + x], z + 0.5f);

                        float dist0 = (highest0 + highest) / (highest - lowest);



                        triangles[t1ArrayPos].p1 = vec3(x + 1, positions[arrayPos + 1].y, z);
                        triangles[t1ArrayPos].p2 = vec3(x + 1, positions[arrayPos + max + 1].y, z + 1);
                        triangles[t1ArrayPos].p3 = vec3(x + 0.5f, centerHeights[z * (max - 1) + x], z + 0.5f);

                        float dist1 = (highest1 + highest) / (highest - lowest);



                        triangles[t2ArrayPos].p1 = vec3(x + 1, positions[arrayPos + max + 1].y, z + 1); //possible
                        triangles[t2ArrayPos].p2 = vec3(x, positions[arrayPos + max].y, z + 1); //possible
                        triangles[t2ArrayPos].p3 = vec3(x + 0.5f, centerHeights[z * (max - 1) + x], z + 0.5f);

                        float dist2 = (highest2 + highest) / (highest - lowest);



                        triangles[t3ArrayPos].p1 = vec3(x, positions[arrayPos + max].y, z + 1);
                        triangles[t3ArrayPos].p2 = vec3(x, positions[arrayPos].y, z);
                        triangles[t3ArrayPos].p3 = vec3(x + 0.5f, centerHeights[z * (max - 1) + x], z + 0.5f);

                        float dist3 = (highest3 + highest) / (highest - lowest);

                        triangles[t0ArrayPos].color = vec4(0, dist0 * 0.5f + 0.1f, dist0 - (dist0 * 0.5f) + 0.25f, 1);
                        triangles[t1ArrayPos].color = vec4(0, dist1 * 0.5f + 0.1f, dist1 - (dist1 * 0.5f) + 0.25f, 1);
                        triangles[t2ArrayPos].color = vec4(0, dist2 * 0.5f + 0.1f, dist2 - (dist2 * 0.5f) + 0.25f, 1);
                        triangles[t3ArrayPos].color = vec4(0, dist3 * 0.5f + 0.1f, dist3 - (dist3 * 0.5f) + 0.25f, 1);

                        triangles[t0ArrayPos].color2 = vec4(1);
                        triangles[t1ArrayPos].color2 = vec4(1);
                        triangles[t2ArrayPos].color2 = vec4(1);
                        triangles[t3ArrayPos].color2 = vec4(1);
                        triangles[t0ArrayPos].color3 = vec4(1);
                        triangles[t1ArrayPos].color3 = vec4(1);
                        triangles[t2ArrayPos].color3 = vec4(1);
                        triangles[t3ArrayPos].color3 = vec4(1);
                        //triangles[t1ArrayPos].color3 = vec4(0.5f);
                        //triangles[]
                        //triangles[t2ArrayPos].color3 = vec4(1);
                        //triangles[t3ArrayPos].color3 = vec4(1);
                        /*triangles[t0ArrayPos].color2 = triangles[t1ArrayPos].color;
                        triangles[t1ArrayPos].color2 = triangles[t2ArrayPos].color;
                        triangles[t2ArrayPos].color2 = triangles[t3ArrayPos].color;
                        triangles[t3ArrayPos].color2 = triangles[t0ArrayPos].color;
                        triangles[t0ArrayPos].color3 = triangles[t1ArrayPos].color2;
                        triangles[t1ArrayPos].color3 = triangles[t2ArrayPos].color2;
                        triangles[t2ArrayPos].color3 = triangles[t3ArrayPos].color2;
                        triangles[t3ArrayPos].color3 = triangles[t0ArrayPos].color2;*/
                    }
                }
            }
            if(wireframeMode == 1) {
                for(length = 1; length <= max; length++) {
                    lineCount = length * (length - 1);

                    for(int z = 0; z < length; z++) {
                        for(int x = 0; x < length - 1; x++) {
                            float y1 = positions[z * max + x].y;
                            float dist1 = (y1 + highest) / (highest - lowest);
                            float y2 = positions[z * max + x + 1].y;
                            float dist2 = (y2 + highest) / (highest - lowest);
                            xLines[x * max + z]._start = vec3(x, positions[z * max + x].y, z);
                            xLines[x * max + z]._end = vec3(x + 1, positions[z * max + x + 1].y, z);
                            xLines[x * max + z]._color = vec4(dist1, 1.0f - dist1, 0, 1);
                            xLines[x * max + z]._color2 = vec4(dist2, 1.0f - dist2, 0, 1);
                        }
                    }

                    for(int z = 0; z < length - 1; z++) {
                        for(int x = 0; x < length; x++) {
                            float y1 = positions[z * max + x].y;
                            float dist1 = (y1 + highest) / (highest - lowest);
                            float y2 = positions[z * max + x + max].y;
                            float dist2 = (y2 + highest) / (highest - lowest);
                            zLines[z * max + x]._start = vec3(x, positions[z * max + x].y, z);
                            zLines[z * max + x]._end = vec3(x, positions[z * max + x + max].y, z + 1);
                            zLines[z * max + x]._color = vec4(dist1, 1.0f - dist1, 0, 1);
                            zLines[z * max + x]._color2 = vec4(dist2, 1.0f - dist2, 0, 1);
                        }
                    }
                }
            }
        }
            }


        camera.update(window.window, deltaTime);

        if(fadeLineShader != nullptr && fadeTriangleShader != nullptr) {

            if(wireframeMode == 0) {
                fadeTriangleShader->use();
                fadeTriangleShader->setMat4("viewProj", camera.proj * camera.view);
                glBindVertexArray(*FadeTriangle::getVAO());
                for(int i=0;i<triangleCount * 4;i++) {
                    //fadeTriangleShader->setVec3("color", triangles[i].color);
                    //fadeTriangleShader->setVec3("color2", triangles[i].color2);
                    //fadeTriangleShader->setVec3("color3", triangles[i].color3);
                    triangles[i].draw(&camera);
                }
            }
            if(wireframeMode == 1) {
                fadeLineShader->use();
                fadeLineShader->setMat4("viewProj", camera.proj * camera.view);
                glBindVertexArray(*FadeLine::getVAO());
                for(int i=0;i<lineCount;i++) {
                    int x = i % max;
                    int z = i / max;
                    if(abs(glm::length(vec2(x, z) - vec2(camera._position.z, camera._position.x))) < drawDistance) {
                        xLines[i].draw(&camera);
                    }
                    if(abs(glm::length(vec2(z, x) - vec2(camera._position.z, camera._position.x))) < drawDistance) {
                        zLines[i].draw(&camera);
                    }
                }
            }
        }




        if(camera.ui && lineShader != nullptr) {
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

    delete [] positions;
    delete [] xLines;
    delete [] zLines;
    delete [] triangles;
    delete [] centerHeights;

    glfwTerminate();
    return 0;
}

int triangleTest() {
    auto window = Window("TRIANGLE!!!");
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSwapInterval(0);
    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));
    Line::loadShader();

    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        float deltaTime = window.update();
        float time = window.getTime();
        camera.update(window.window, deltaTime);
        Triangle::loadShader();

        triangleShader->use();
        triangleShader->setMat4("viewProj", camera.proj * camera.view);
        Triangle triangle  = Triangle(vec3(0, 0, 0), vec3(1, sin(time*3), 0), vec3(0, cos(time*3), 1), vec4(1, 0, 0, 1));
        /*Triangle triangle2 = Triangle(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), vec4(0, 1, 0, 1));
        Triangle triangle3 = Triangle(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), vec4(0, 0, 1, 1));
        Triangle triangle4 = Triangle(vec3(0, 0, 0), vec3(0, 0, 0), vec3(1, 1, 1), vec4(1, 1, 0, 1));*/
        triangle.draw(&camera, true);
        /*triangle2.draw(&camera, false);
        triangle3.draw(&camera, false);
        triangle4.draw(&camera, false);*/

        if(camera.ui && lineShader != nullptr) {
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

    return 0;
}
int geometry() {

    auto window = Window("TRIANGLE!!!");
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSwapInterval(0);
    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));
    Line::loadShader();

    struct Vertex {
        vec3 pos;
        vec3 normal;
        vec2 uv;
    };

    int subDiv = 3;
    float width = 5.0f;
    float height = 5.0f;
    for(int row = 0; row <= subDiv; row++) {
        for(int col = 0; col <= subDiv; col++) {
            Vertex v;
            v.uv.x = static_cast<float>(col) / static_cast<float>(subDiv);
            v.uv.y = static_cast<float>(row) / static_cast<float>(subDiv);
            v.pos.x = v.uv.x * width;
            v.pos.y = v.uv.y * height;
            v.pos.z = 0.0f;
            v.normal = vec3(0, 0, 1);
        }
    }

    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        float deltaTime = window.update();
        float time = window.getTime();
        camera.update(window.window, deltaTime);

        if(camera.ui && lineShader != nullptr) {
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

    return 0;
}

int skybox() {
    auto window = Window("Skybox :D");
    glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSwapInterval(0);
    camera = Camera(vec3(0.5f, 12.0f, 31.3f), vec3(0, 0, -450.0f), 60.0f, vec2(Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT));
    Line::loadShader();


    //Assimp::Importer importer;
    //const aiScene* scene = importer.ReadFile("assets/testing/backpack.fbx", aiProcess_Triangulate | aiProcess_FlipUVs);

    Shader shader = Shader("assets/testing/sphere");
    string path = "assets/testing/backpack/backpack.obj";
    Model ourModel(path);

    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        float deltaTime = window.update();
        float time = window.getTime();
        camera.update(window.window, deltaTime);

        shader.use();

        shader.setMat4("viewProj", camera.proj * camera.view);
        shader.setMat4("model", mat4(1));
        //ourModel.Draw(shader);


        if(camera.ui && lineShader != nullptr) {
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

    return 0;
}

constexpr int WORLD_RUNCODE = 0;
constexpr int MESH_RUNCODE = 1;
constexpr int TRIANGLE_RUNCODE = 2;
constexpr int GEOMETRY_RUNCODE = 3;
constexpr int SKYBOX = 4;

int main() {

    int runCode = SKYBOX;
    switch(runCode) {
        case WORLD_RUNCODE: return world();
        case MESH_RUNCODE: return mesh();
        case TRIANGLE_RUNCODE: return triangleTest();
        case GEOMETRY_RUNCODE: return geometry();
        case SKYBOX: return skybox();
    }


}