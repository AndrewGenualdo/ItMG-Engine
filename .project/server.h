//
// Created by cobble on 12/27/2024.
//

#ifndef ENGINE_2_SERVER_H
#define ENGINE_2_SERVER_H

#pragma once

#include <cstdio>
#include "enet/enet.h"
#include "client.h"
#include "player.h"

class Server {
public:

    static std::vector<Client::PlayerPacket> packetQueue;
    static ENetPeer *clients[Player::MAX_PLAYERS];
    static int playerCount;
    static bool startServer;
    static bool isLan;

    static void sendPacket(ENetPeer *peer, const Client::ChatPacket& data);

    static void sendPacket(ENetPeer *peer, Client::PlayerPacket data);

    static void sendPacket(ENetPeer *peer, Client::ServerPacket data);

    [[noreturn]] [[noreturn]] static int run();
};


#endif //ENGINE_2_SERVER_H
