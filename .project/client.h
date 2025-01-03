//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_CLIENT_H
#define ENGINE_2_CLIENT_H

#define CHANNEL_GAME 0
#define CHANNEL_CHAT 1

#pragma once

#include <cstdio>
#include <vector>
#include "enet/enet.h"
#include "player.h"

class Client {
public:

    static const int KEY_COUNT = 5;

    struct PlayerPacket {
        float x, y;
        float xVel, yVel;
        float mx, my;
        bool keys[KEY_COUNT]{};
        int id;

        explicit PlayerPacket(int id) {
            x = Window::GAME_WIDTH / 2;
            y = Window::GAME_HEIGHT / 2;
            xVel = 0;
            yVel = 0;
            this->id = id;
            for (bool &key: keys) {
                key = false;
            }
        }
    };

    struct ServerPacket {
        bool players[Player::MAX_PLAYERS]{};
        int id = 0;

        ServerPacket() = default;

        explicit ServerPacket(const bool players[Player::MAX_PLAYERS]) {
            for(int i=0;i<Player::MAX_PLAYERS;i++) {
                this->players[i] = players[i];
            }
        }
    };

    struct ChatPacket {
        float time{};
        int ownerId{};
        std::string contents;

        ChatPacket() = default;
        ChatPacket(float time, int ownerId, std::string contents) {
            this->time = time;
            this->ownerId = ownerId;
            this->contents = contents;
        }

        [[nodiscard]] std::vector<char> serialize() const {
            std::vector<char> buffer;

            // Serialize the time (float)
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&time), reinterpret_cast<const char*>(&time) + sizeof(time));

            // Serialize the ownerId (int)
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&ownerId), reinterpret_cast<const char*>(&ownerId) + sizeof(ownerId));

            // Serialize the length of the contents string
            int contentsLength = contents.size();
            buffer.insert(buffer.end(), reinterpret_cast<const char*>(&contentsLength), reinterpret_cast<const char*>(&contentsLength) + sizeof(contentsLength));

            // Serialize the contents (string)
            buffer.insert(buffer.end(), contents.begin(), contents.end());

            return buffer;
        }

        static ChatPacket deserialize(const char* data, size_t length) {
            ChatPacket packet;

            // Pointer to the current position in the raw data
            const char* ptr = data;

            // Read the time (float)
            std::memcpy(&packet.time, ptr, sizeof(float));
            ptr += sizeof(float);

            // Read the ownerId (int)
            std::memcpy(&packet.ownerId, ptr, sizeof(int));
            ptr += sizeof(int);

            // Read the length of the contents string (int)
            int contentsLength = 0;
            std::memcpy(&contentsLength, ptr, sizeof(int));
            ptr += sizeof(int);

            // Ensure we don't exceed the packet size (check contentsLength vs remaining data)
            if (length < (ptr - data) + contentsLength) {
                // Handle the error (e.g., not enough data to read the full string)
                std::cerr << "Not enough data to read string contents!" << std::endl;
                return packet;  // Return the empty packet
            }

            // Read the contents (string)
            packet.contents.assign(ptr, contentsLength);

            return packet;
        }
    };

    static bool debug;
    static bool connected;
    static PlayerPacket clientPacket;
    static ENetPeer *peer;
    static std::vector<PlayerPacket> packetQueue;
    static int id;
    static int playerCount;
    static std::string ip;
    static bool *paused;
    static ENetHost *client;
    static bool hostClient;
    static std::vector<Client::ChatPacket> history;
    static float pingTime;
    static float pongTime;

    static bool updateKeyPress(int key, bool newValue);

    static float updateMousePos(bool isX, float value);

    static void setPos(float x, float y);

    static void setVel(float xVel, float yVel);

    static void setMousePos(float x, float y);

    static int run();

    static void sendPacket(const ChatPacket& data);

    static void sendPacket(PlayerPacket data);

};

#endif //ENGINE_2_CLIENT_H
