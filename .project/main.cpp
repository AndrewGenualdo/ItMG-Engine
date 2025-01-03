#include <cstdio>
#include "cobb/core/window.h"
#include "ew/external/glad.h"
#include "ew/ewMath/ewMath.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "cobb/core/camera.h"
#include "cobb/shapes/line.h"
#include "cobb/misc/texture2d.h"
#include "cobb/misc/animation.h"
#include "player.h"
#include "server.h"
#include "client.h"
#include "cobb/misc/fontRenderer.h"
#include "chatBox.h"
#include <thread>

using namespace cobb;

constexpr static int SCENE_MENU = 0;
constexpr static int SCENE_GAME = 1;

static int scene = SCENE_MENU;

static Camera camera;
static Window window;
static ChatBox chatBox;
bool editMode = false;
bool borderless = false;


/*void drawAxisGizmo() {
	if(camera.ui && lineShader != nullptr) {
		glDisable(GL_DEPTH_TEST);

		//higher number = less stuttering, can't be above ~950 because depth will start cutting it off, 900 to be safe
		const float compassScale = 900.0f;

		vec3 p = camera.forward * vec3(compassScale) + camera.pos;

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
}*/


static void key_callback(GLFWwindow *w, int key, int scancode, int action, int mods) {
    camera.handleKeyboard(key, action);
    if (action == GLFW_PRESS) {
        if (chatBox.handleKeyboard(key, mods) == RETURN) return;
        if (key == GLFW_KEY_ESCAPE) {
            if (camera.lock) {
                glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
                glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glClearColor(0.0f, 0.05f, 0.1f, 1.0f);
                //glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        } else if (key == GLFW_KEY_P) {
            std::cout << camera.getString() << std::endl;
        } else if (key == GLFW_KEY_F12) {
            Client::debug = !Client::debug;
        } else if (key == GLFW_KEY_F8) {
            editMode = !editMode;
            if (editMode) {
                camera.lock = true;
                glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                camera.lock = false;
                //glfwSetInputMode(w, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        } else if (key == GLFW_KEY_F11) {
            borderless = !borderless;
            if (borderless) {
                GLFWmonitor *monitor = glfwGetPrimaryMonitor();
                const GLFWvidmode *mode = glfwGetVideoMode(monitor);
                glfwSetWindowMonitor(window.window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
                Window::setVsync(true);
            } else {
                glfwSetWindowMonitor(window.window, nullptr, 100, 100, Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT, GLFW_DONT_CARE);
                Window::setVsync(true);
            }
        }
    }
}

void mouse_position_callback(GLFWwindow *w, double xpos, double ypos) {
    camera.handleMouse(xpos, ypos);
    window.mousePos.x = static_cast<float>(xpos);
    window.mousePos.y = static_cast<float>(ypos);
}

void scroll_callback(GLFWwindow *w, double xoffset, double yoffset) {
    //camera.handleScroll(yoffset);
}

void framebuffer_size_callback(GLFWwindow *w, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    window.setWidth(width);
    window.setHeight(height);
    glViewport(0, 0, width, height);
    Texture2d::setOrtho(glm::ortho(0.0f, window.getWidth(), 0.0f, window.getHeight(), -1.0f, 1.0f));
    if (Client::debug) std::cout << "Window resized to: " << window.getWidth() << "x" << window.getHeight() << "." << std::endl;
    window.getGameDimensions();
    if (Client::debug) std::cout << "Game window resized to: " << window.gw << "x" << window.gh << "." << std::endl;
}

int main() {
    window = Window("Platformer!!!");
    Window::setVsync(true);
    //glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, mouse_position_callback);
    glfwSetScrollCallback(window.window, scroll_callback);
    glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
    camera = Camera(vec3(), vec3(), 60.0f, vec2(window.getWidth(), window.getHeight()));
    Client::paused = &camera.lock;
    //Client::debug = true;

    for (int i = 0; i < Player::MAX_PLAYERS; i++) {
        Player::players[i] = Player(&window, "assets/textures/guy/", 0.5f);
        Player::players[i].id = i;
    }
    std::thread serv = std::thread(Server::run);
    std::thread clie = std::thread(Client::run);

    Texture2d::loadShader();
    FontRenderer::loadShader();
    Texture2d::window = &window;
    Texture2d::setOrtho(glm::ortho(0.0f, Window::SCREEN_WIDTH, 0.0f, Window::SCREEN_HEIGHT, -1.0f, 1.0f));
    FontRenderer fontRenderer = FontRenderer("assets/textures/font/font.png");
    for (int i = 0; i <= Player::MAX_PLAYERS; i++) fontRenderer.colors[i] = Player::playerColors[i];
    chatBox = ChatBox(&fontRenderer, 2.0f);


    Texture2d platform = Texture2d("assets/textures/ui/blank.png");
    Texture2d clipboard = Texture2d("assets/textures/ui/clipboard.png");


    Player::levels[Player::currentLevel].load("assets/levels/1.txt");
    bool wasMouseDown = false;
    bool wasRightMouseDown = false;
    vec2 mouseDownStart = vec2(-1);

    //Render loop
    while (!glfwWindowShouldClose(window.window)) {
        glfwPollEvents();
        float deltaTime = window.update();
        camera.update(window.window, deltaTime);
        mat4 viewProj = camera.proj * camera.view;
        bool mouseDown = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_LEFT);
        bool rightMouseDown = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_RIGHT);
        bool mousePressed = mouseDown && !wasMouseDown;
        bool rightMousePressed = rightMouseDown && !wasRightMouseDown;
        bool mouseReleased = wasMouseDown && !mouseDown;
        bool rightMouseReleased = wasRightMouseDown && !rightMouseDown;
        wasMouseDown = mouseDown;
        wasRightMouseDown = rightMouseDown;
        if (mousePressed || rightMousePressed) mouseDownStart = vec2(window.mousePos.x, window.mousePos.y);


        //background
        Texture2d::setColor(vec4(0.0f, 0.1f, 0.2f, 1.0f));
        platform.drawRaw(window.gx, window.gy, window.gw, window.gh);

        //draw platforms
        int platCounter = 0;
        for (auto &plat: Player::levels[Player::currentLevel].platforms) {
            int pc = 0;
            vec4 color = vec4(1);
            //count how many players are on current platform
            for (auto &player: Player::players) if (player.platStandingOn == platCounter) pc++;
            if (pc > 0) { //if its more then 0, add their colors together to make the platform color
                color = vec4(0);
                for (int i = 0; i < Player::MAX_PLAYERS; i++) if (platCounter == Player::players[i].platStandingOn) color += Player::playerColors[i] / static_cast<float>(pc);
            }

            plat.draw(color);
            platCounter++;
        }

        Texture2d::setColor(vec4(sin(window.getTime() * 1.5f + glm::radians(240.0f)) * 0.5f + 0.5f, sin(window.getTime() * 1.5f + glm::radians(120.0f)) * 0.5f + 0.5f, sin(window.getTime() * 1.5f) * 0.5f + 0.5f, 1.0f));
        platform.draw(Window::GAME_WIDTH / 2.0f - 50.0f, 0.0f, 100.0f, 100.0f);

        std::string connectedPlayers;

        //players
        for (int i = 0; i < Player::MAX_PLAYERS; i++) {
            connectedPlayers += Player::players[i].connected ? "1" : "0";
            if (!Player::players[i].connected) continue;
            //handle input
            if (i == Client::id && !camera.lock && !chatBox.open) {
                bool mouseLeft = glfwGetMouseButton(window.window, GLFW_MOUSE_BUTTON_LEFT);
                bool left = glfwGetKey(window.window, GLFW_KEY_A);
                bool jump = glfwGetKey(window.window, GLFW_KEY_SPACE) || glfwGetKey(window.window, GLFW_KEY_W);
                bool down = glfwGetKey(window.window, GLFW_KEY_S) || glfwGetKey(window.window, GLFW_KEY_LEFT_CONTROL);
                bool right = glfwGetKey(window.window, GLFW_KEY_D);
                float mx = window.gX(window.mousePos.x);
                float my = window.gY(window.getHeight() - window.mousePos.y);

                if (Player::players[i].mousePos.x != mx) Player::players[i].mousePos.x = Client::updateMousePos(true, mx);
                if (Player::players[i].mousePos.y != my) Player::players[i].mousePos.y = Client::updateMousePos(false, my);
                if (Player::players[i].keys[MOUSE_LEFT] != mouseLeft) Player::players[i].keys[MOUSE_LEFT] = Client::updateKeyPress(MOUSE_LEFT, mouseLeft);
                if (Player::players[i].keys[KEY_LEFT] != left) Player::players[i].keys[KEY_LEFT] = Client::updateKeyPress(KEY_LEFT, left);
                if (Player::players[i].keys[KEY_JUMP] != jump) Player::players[i].keys[KEY_JUMP] = Client::updateKeyPress(KEY_JUMP, jump);
                if (Player::players[i].keys[KEY_DOWN] != down) Player::players[i].keys[KEY_DOWN] = Client::updateKeyPress(KEY_DOWN, down);
                if (Player::players[i].keys[KEY_RIGHT] != right) Player::players[i].keys[KEY_RIGHT] = Client::updateKeyPress(KEY_RIGHT, right);
            }

            //player physics
            if (!(camera.lock && i == Client::id)) Player::players[i].update(deltaTime, Player::levels[Player::currentLevel]);

            //draw players
            Player::players[i].draw();
        }


        if(camera.lock) {
            Texture2d::setColor(vec4(0, 0, 0, 0.5f));
            platform.draw(0, 0, Window::GAME_WIDTH, Window::GAME_HEIGHT);
        }




        //font setup
        fontRenderer.setColor(Player::playerColors[Client::id]);
        float fontScale = 3.0f;
        float mx = window.gX(window.mousePos.x);
        float my = window.gY(window.getHeight() - window.mousePos.y);

        //click IP to copy it
        float ipx1 = 10;
        float ipx2 = ipx1 + fontScale * fontRenderer.getWidth("ip: " + Client::ip);
        float ipy1 = Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10 - (4.0f * (fontScale * (fontRenderer.getHeight() + (2.5f * fontRenderer.characterSpacing))));
        float ipy2 = ipy1 + fontScale * fontRenderer.getHeight();
        if (mx >= ipx1 - 5 && mx <= ipx2 + 5 && my >= ipy1 - 5 && my <= ipy2 + 5) {
            Texture2d::setColor(mouseDown ? vec4(0, 0, 0, 0.4f) : vec4(0, 0, 0, 0.2f));
            platform.draw(ipx1 - 5, ipy1 - 5, ipx2 - ipx1 + 10, ipy2 - ipy1 + 10);
            if (mousePressed) glfwSetClipboardString(window.window, Client::ip.c_str());
        }

        //draw misc text
        fontRenderer.draw(
                "x: " + std::to_string(camera.lock ? (int) mx : (int) Player::players[Client::id].pos.x) +
                "\ny: " + std::to_string(camera.lock ? (int) my : (int) Player::players[Client::id].pos.y) +
                "\nxVel: " + std::to_string((int) Player::players[Client::id].velocity.x) +
                "\nyVel: " + std::to_string((int) Player::players[Client::id].velocity.y) +
                "\nip: " + Client::ip, 10, Window::GAME_HEIGHT - fontRenderer.getHeight() * fontScale - 10, fontScale);

        //debug mode data
        std::string debugString;
        if (Client::debug) {
            debugString.append("\n%1Debug mode enabled\nid: " + std::to_string(Client::id) +
                               "\n[" + connectedPlayers + "]" +
                               "\nHost: " + (Client::hostClient ? "true" : "false") + "\n");
        }
        if (Client::id == Player::MAX_PLAYERS) {
            debugString.append("\n%3Controls\nFullscreen: F11\nDebug: F12\nBuild Mode: F8\nMovement: A, D, LCTRL/S, and W/Space\nOpen Chat: T\n\n%4Commands:\n/host\n/join [ip]\n/leave\n/ping\n");
        }

        //edit mode
        if (editMode) {
            //edit mode place platform
            Platform newPlatform = Platform(0, 0, 0, 0);
            if (mouseDownStart.x != -1) { //actually drawing plat
                if (mouseDown || mouseReleased) {
                    float npx = window.gX(mouseDownStart.x); //new platform x
                    float npy = window.gY(window.getHeight() - mouseDownStart.y); //new platform y
                    newPlatform.x = npx;
                    newPlatform.y = npy;
                    newPlatform.w = mx - npx;
                    newPlatform.h = my - npy;
                    if (newPlatform.w < 0) {
                        newPlatform.w *= -1;
                        newPlatform.x -= newPlatform.w;
                    }
                    if (newPlatform.h < 0) {
                        newPlatform.h *= -1;
                        newPlatform.y -= newPlatform.h;
                    }
                    newPlatform.solid = true;

                    if (newPlatform.w > 0 && newPlatform.h > 0) {
                        newPlatform.draw(Player::playerColors[Player::MAX_PLAYERS] * 0.5f);
                    }
                    if (mouseReleased) {
                        Player::levels[Player::currentLevel].platforms.push_back(newPlatform);
                    }
                } else if (rightMouseDown || rightMouseReleased) {
                    float npx = window.gX(mouseDownStart.x); //new platform x
                    float npy = window.gY(window.getHeight() - mouseDownStart.y); //new platform y
                    newPlatform.x = npx;
                    newPlatform.y = npy;
                    newPlatform.w = mx - npx;
                    newPlatform.h = my - npy;
                    if (newPlatform.w < 0) {
                        newPlatform.w *= -1;
                        newPlatform.x -= newPlatform.w;
                    }

                    if (newPlatform.h != 0 && abs(newPlatform.h) != 5) newPlatform.h = 5 * (newPlatform.h / abs(newPlatform.h));

                    if (newPlatform.h < 0) {
                        newPlatform.h *= -1;
                        newPlatform.y -= newPlatform.h;
                    }


                    newPlatform.solid = false;

                    if (newPlatform.w > 0 && newPlatform.h > 0) {
                        newPlatform.draw(Player::playerColors[Player::MAX_PLAYERS] * 0.5f);
                    }
                    if (rightMouseReleased) {
                        Player::levels[Player::currentLevel].platforms.push_back(newPlatform);
                    }
                }

            }

            //edit mode data
            int platHovering = -1;

            for (int i = 0; i < Player::levels[Player::currentLevel].platforms.size(); i++) {
                Platform *p = &Player::levels[Player::currentLevel].platforms[i];
                if (mx >= p->x && mx <= p->x + p->w && my >= p->y && my <= p->y + p->h) {
                    platHovering = i;
                    p->draw(vec4(Player::playerColors[Client::id]));
                    if (glfwGetKey(window.window, GLFW_KEY_BACKSPACE)) {
                        Player::levels[Player::currentLevel].platforms.erase(Player::levels[Player::currentLevel].platforms.begin() + i);
                    }
                    break;
                }
            }
            debugString.append("\n%" + std::to_string(Player::MAX_PLAYERS) + "Edit mode enabled\nPlatform ID: " + (mouseDownStart.x == -1 ? (platHovering == -1 ? "None" : std::to_string(platHovering)) : "New") +
                               "\nPlatform Data: " + (mouseDownStart.x == -1 ? (platHovering != -1 ? Player::levels[Player::currentLevel].platforms[platHovering].to_string() : "None") : newPlatform.to_string()));
        }

        if (!debugString.empty()) {
            fontRenderer.setColor(Player::playerColors[(Client::id + 1) % Player::MAX_PLAYERS]);
            fontRenderer.draw(debugString, ipx1, ipy1 - (fontScale * (fontRenderer.getHeight() + (2.5f * fontRenderer.characterSpacing))), 3.0f);
        }

        //render chat
        if(!Client::history.empty()) {
            Texture2d::setColor(vec4(0.05f, 0.05f, 0.1f, chatBox.open ? 0.8f : 0.4f));
            platform.draw(0, 0, (chatBox.getWidth() + 8) * chatBox.scale, ((fontRenderer.getHeight() + (2.5f * fontRenderer.characterSpacing)) * static_cast<float>(Client::history.size() + 1) + 7) * chatBox.scale);
        }

        chatBox.draw();

        if (mouseReleased || rightMouseReleased) mouseDownStart = vec2(-1);

        glfwSwapBuffers(window.window);
    }
    printf("Shutting down...");
}
