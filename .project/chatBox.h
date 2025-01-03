//
// Created by cobble on 12/31/2024.
//

#ifndef ENGINE_2_CHATBOX_H
#define ENGINE_2_CHATBOX_H
#define RETURN 1
#define CONTINUE 0


#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include "cobb/misc/fontRenderer.h"
#include "client.h"
#include "server.h"

using namespace cobb;

class ChatBox {
    std::string currentMessage;
    FontRenderer *fontRenderer{};
public:
    float scale{};
    bool open{};

    ChatBox() = default;
    explicit ChatBox(FontRenderer *fontRenderer, float scale) {
        open = false;
        this->scale = scale;
        currentMessage = "";
        this->fontRenderer = fontRenderer;
    }

    void draw() {
        for(int i=(int) Client::history.size() - 1; i >= 0; i--) {
            if(Client::history[i].time + 10.0f > glfwGetTime() || open) {
                vec4 c;
                if(Client::history[i].ownerId == -1) {
                    c = Player::playerColors[Client::id];
                    c.r *= 0.5f;
                    c.g *= 0.5f;
                    c.b *= 0.5f;
                } else {
                    c = Player::playerColors[Client::history[i].ownerId];
                }
                if(Client::history[i].time + 9.0f > glfwGetTime()) {
                    fontRenderer->setColor(c);
                } else {

                    float t = open ? 1.0f : Client::history[i].time + 10.0f - static_cast<float>(glfwGetTime());
                    fontRenderer->setColor(vec4(c.r, c.g, c.b, t));
                }

                fontRenderer->draw(Client::history[i].contents, 15, 15 + ((static_cast<float>(Client::history.size() - i)) * (fontRenderer->getHeight() + fontRenderer->characterSpacing * 2.5f) * scale), scale);
            }
        }
        if(!open) return;
        fontRenderer->setColor(Player::playerColors[Client::id]);
        fontRenderer->draw(currentMessage, 3 * scale, 3 * scale, scale);
    }

    static int handleCommands(std::string *input) {
        *input = input->substr(1); //remove slash
        std::string cmd = input->find_first_of(' ') == std::string::npos ? *input : input->substr(0, input->find_first_of(' '));
        std::string args = input->find_first_of(' ') == std::string::npos ? "" : input->substr(input->find_first_of(' ') + 1);

        auto chatPacket = Client::ChatPacket();
        chatPacket.time = static_cast<float>(glfwGetTime());
        chatPacket.ownerId = Player::MAX_PLAYERS;; //system

        if(cmd == "ping") {
            if(Client::id == Player::MAX_PLAYERS) {
                chatPacket.contents = "[SYSTEM] You must be conneted to a server to use this!";
                Client::history.push_back(chatPacket);
                return RETURN;
            } else {
                *input = "/ping";
                Client::pingTime = static_cast<float>(glfwGetTime());
                return CONTINUE;
            }
        } else if(cmd == "host") {
            if(Client::id == Player::MAX_PLAYERS) {
                Server::isLan = args != "lan";
                Server::startServer = true;
            } else {
                chatPacket.contents = "[SYSTEM] You are already connected to a server!";
                Client::history.push_back(chatPacket);
            }

            return RETURN;
        } else if(cmd == "join") {
            if(Client::id == Player::MAX_PLAYERS) {
                Client::ip = args;
                Client::ip.erase(remove(Client::ip.begin(), Client::ip.end(), ' '), Client::ip.end());
            } else {
                chatPacket.contents = "[SYSTEM] You are already connected to a server!";
                Client::history.push_back(chatPacket);
            }
            return RETURN;
        } else if(cmd == "leave") {
            chatPacket.contents = "[SYSTEM] You have disconnected from server with ip: " + Client::ip;
            Client::connected = false;
            Client::id = Player::MAX_PLAYERS;
            Client::ip.clear();
            for(auto & player : Player::players) {
                player.connected = false;
                player.platStandingOn = -1;
            }
            Client::history.push_back(chatPacket);
            return RETURN;
        } else if(cmd == "save") {
            Player::levels[Player::currentLevel].save("assets/levels/"+args);
            chatPacket.contents = "[SYSTEM] Saved level to file: assets/levels/"+args+"";
            Client::history.push_back(chatPacket);
            return RETURN;
        } else if(cmd == "load") {
            Player::levels[Player::currentLevel].load("assets/levels/"+args);
            chatPacket.contents = "[SYSTEM] Loaded level from file: assets/levels/"+args+"";
            Client::history.push_back(chatPacket);
            return RETURN;
        } else {
            chatPacket.contents = "[SYSTEM] Unknown command \"" + cmd + "\"";
            Client::history.push_back(chatPacket);
            return RETURN;
        }
    }

    void sendMessage() {
        Client::ChatPacket chatPacket = Client::ChatPacket(static_cast<float>(glfwGetTime()), Client::id, currentMessage);
        if(chatPacket.contents[0] == '/') {
            if(handleCommands(&chatPacket.contents) == RETURN) return;
        } else {
            Client::history.emplace_back(glfwGetTime(), Client::id, currentMessage);
        }
        Client::sendPacket(chatPacket);
        currentMessage.clear();
    }

    static std::string getCharacterFromKey(int key, int mods) {
        // Base key to shifted character map
        static std::unordered_map<int, char> shiftedCharacters = {
                {GLFW_KEY_1, '!'}, {GLFW_KEY_2, '@'}, {GLFW_KEY_3, '#'}, {GLFW_KEY_4, '$'}, {GLFW_KEY_5, '%'},
                {GLFW_KEY_6, '^'}, {GLFW_KEY_7, '&'}, {GLFW_KEY_8, '*'}, {GLFW_KEY_9, '('}, {GLFW_KEY_0, ')'},
                {GLFW_KEY_MINUS, '_'}, {GLFW_KEY_EQUAL, '+'}, {GLFW_KEY_LEFT_BRACKET, '{'}, {GLFW_KEY_RIGHT_BRACKET, '}'}, {GLFW_KEY_SEMICOLON, ':'},
                {GLFW_KEY_APOSTROPHE, '"'}, {GLFW_KEY_COMMA, '<'}, {GLFW_KEY_PERIOD, '>'}, {GLFW_KEY_SLASH, '?'}, {GLFW_KEY_BACKSLASH, '|'},
                {GLFW_KEY_GRAVE_ACCENT, '~'}
        };

        const char* keyName = glfwGetKeyName(key, 0);
        if (!keyName) {
            // Non-printable key
            return "";
        }

        bool isShift = mods & GLFW_MOD_SHIFT;
        char baseChar = keyName[0];

        // If Shift is held and this is a shifted character
        if (isShift && shiftedCharacters.find(key) != shiftedCharacters.end()) {
            return std::string(1, shiftedCharacters[key]);
        }

        // Handle uppercase letters
        if (isShift && baseChar >= 'a' && baseChar <= 'z') {
            baseChar -= 32; // Convert to uppercase
        }

        return std::string(1, baseChar);
    }

    int handleKeyboard(int key, int mods) {
        if (!open) {
            if (key == GLFW_KEY_T) {
                open = true;
                currentMessage.clear();
                //glfwSetInputMode(Player::window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                return RETURN;
            } else if(key== GLFW_KEY_SLASH) {
                open = true;
                currentMessage = "/";
                glfwSetInputMode(Player::window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                return RETURN;
            }
            return CONTINUE;
        } else {
            bool ctrlMod = mods & GLFW_MOD_CONTROL;

            if (key == GLFW_KEY_ESCAPE) {
                open = false;
                *Client::paused = false;
                //glfwSetInputMode(Player::window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                return RETURN;
            } else if(key == GLFW_KEY_ENTER) {
                if (!currentMessage.empty()) sendMessage();
                open = false;
                *Client::paused = false;
                //glfwSetInputMode(Player::window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                return RETURN;
            } else if(key == GLFW_KEY_BACKSPACE) {
                if(ctrlMod) {
                    size_t lastSpace = currentMessage.find_last_of(' ');
                    if(lastSpace != std::string::npos) { //no space
                        currentMessage.erase(lastSpace);
                    } else {
                        currentMessage.clear();
                    }
                } else {
                    if(!currentMessage.empty()) currentMessage.erase(currentMessage.length() - 1);
                }

                return RETURN;
            } else if(key == GLFW_KEY_SPACE) {
                currentMessage+=" ";
                return RETURN;
            } else if(ctrlMod && key == GLFW_KEY_V) {
                const char *clipboardContent = glfwGetClipboardString(Player::window->window);
                if (clipboardContent) {
                    currentMessage += clipboardContent; // Append clipboard content to your current text
                } else {
                    if(Client::debug) std::cerr << "Clipboard is empty or inaccessible." << std::endl;
                }
                return RETURN;
            }
            currentMessage += getCharacterFromKey(key, mods);
            return RETURN;
        }
    }

    float getWidth() {
        float longest = fontRenderer->getWidth(currentMessage);
        for(auto & chatPacket : Client::history) {
            float len = fontRenderer->getWidth(chatPacket.contents) + 3;
            if(len > longest) longest = len;
        }
        return longest;
    }
};


#endif //ENGINE_2_CHATBOX_H
