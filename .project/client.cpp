//
// Created by cobble on 12/28/2024.
//
#include <iostream>
#include <cstring>
#include <thread>
#include "client.h"
#include "player.h"

bool Client::debug = false;
bool Client::connected = false;
Client::PlayerPacket Client::clientPacket = PlayerPacket(0);
ENetPeer *Client::peer;
std::vector<Client::PlayerPacket> Client::packetQueue;
int Client::id = Player::MAX_PLAYERS;
int Client::playerCount = 0;
std::string Client::ip;
bool *Client::paused = nullptr;
ENetHost *Client::client;
bool Client::hostClient = false;
std::vector<Client::ChatPacket> Client::history;
float Client::pingTime = 0.0f;
float Client::pongTime = 0.0f;


bool Client::updateKeyPress(int key, bool newValue) {
    clientPacket.keys[key] = newValue;
    packetQueue.push_back(clientPacket);
    return newValue;
}

float Client::updateMousePos(bool isX, float value) {
    if(isX) {
        clientPacket.mx = value;
    } else {
        clientPacket.my = value;
    }
    packetQueue.push_back(clientPacket);
    return value;
}

void Client::setPos(float x, float y) {
    clientPacket.x = x;
    clientPacket.y = y;
}

void Client::setVel(float xVel, float yVel) {
    clientPacket.xVel = xVel;
    clientPacket.yVel = yVel;
}

void Client::setMousePos(float x, float y) {
    clientPacket.mx = x;
    clientPacket.my = y;
}

int Client::run() {
    if (enet_initialize() != 0) {
        fprintf(stderr, "[CLIENT] An error occurred while initializing ENet!\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    client = enet_host_create(nullptr, 1, 2, 0, 0);

    if (client == nullptr) {
        fprintf(stderr, "[CLIENT] An error occurred while trying to create an ENet client host\n");
        return EXIT_FAILURE;
    }

    ENetAddress address; //ip address and port of server connecting to
    ENetEvent event; //events received (data received mostly)

    while(true) {
        while(Client::ip.empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        enet_address_set_host(&address, ip.c_str()); //CHANGE THIS TO OTHER PEOPLE'S IPs TO CONNECT
        address.port = 881;
        peer = enet_host_connect(client, &address, 2, 0);
        if (peer == nullptr) {
            fprintf(stderr, "[CLIENT] No available peers for initiating an ENet connection\n");
            continue;
        }

        auto lastPacket = static_cast<float>(glfwGetTime());
        float timeBetweenMovingPackets = 0.05f;
        float maxTimeBetweenPackets = 1.0f;

        //making sure we connected to server
        if (enet_host_service(client, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT) {
            printf("[CLIENT] Connection to %s:%i succeeded!\n",
                   ip.c_str(),
                   address.port);
            connected = true;
            playerCount = 1;

            auto chatPacket = ChatPacket();
            chatPacket.time = lastPacket;
            chatPacket.contents = "[SYSTEM] Successfully connected to server with ip: " + Client::ip;
            chatPacket.ownerId = Player::MAX_PLAYERS;
            history.push_back(chatPacket);
        } else {
            enet_peer_reset(peer);
            fprintf(stderr, "[CLIENT] Connection to %s:%i failed!\n",
                    ip.c_str(),
                    address.port);

            auto chatPacket = ChatPacket();
            chatPacket.time = lastPacket;
            chatPacket.contents = "[SYSTEM] Failed to connect to server with ip: " + Client::ip;
            chatPacket.ownerId = Player::MAX_PLAYERS;
            history.push_back(chatPacket);
            Client::ip.clear();
            continue;
        }


        // GAME LOOP START
        while (connected) { //1000 is ms between packet receives
            while (enet_host_service(client, &event, 15) > 0) {
                switch (event.type) {
                    case ENET_EVENT_TYPE_RECEIVE: {
                        if(Client::debug) printf("[CLIENT] A packet of length %zu containing %s was received from %x:%u on channel %u.\n",
                                                 event.packet->dataLength,
                                                 event.packet->data,
                                                 event.peer->address.host,
                                                 event.peer->address.port,
                                                 event.channelID);
                        if(event.channelID == CHANNEL_GAME) {
                            switch (event.packet->dataLength) {
                                case sizeof(Client::ServerPacket): {
                                    auto packetData = Client::ServerPacket();
                                    std::memcpy(&packetData, event.packet->data, sizeof(packetData));
                                    playerCount = 0;
                                    std::string playerString;
                                    for(int i=0;i<Player::MAX_PLAYERS;i++) {
                                        if(Player::players[i].connected && !packetData.players[i]) { //disconnected
                                            auto chatPacket = ChatPacket();
                                            chatPacket.ownerId = Player::MAX_PLAYERS;
                                            chatPacket.contents = "[SYSTEM] %"+std::to_string(i)+"Player "+std::to_string(i + 1)+"%"+std::to_string(Player::MAX_PLAYERS)+" has disconnected!";
                                            chatPacket.time = static_cast<float>(glfwGetTime());
                                            history.push_back(chatPacket);
                                        } else if(!Player::players[i].connected && packetData.players[i]) {//connected
                                            auto chatPacket = ChatPacket();
                                            chatPacket.ownerId = Player::MAX_PLAYERS;
                                            chatPacket.contents = "[SYSTEM] %"+std::to_string(i)+"Player "+std::to_string(i + 1)+"%"+std::to_string(Player::MAX_PLAYERS)+" has connected!";
                                            chatPacket.time = static_cast<float>(glfwGetTime());
                                            history.push_back(chatPacket);
                                        }
                                        Player::players[i].connected = packetData.players[i];
                                        if(Player::players[i].connected) {
                                            playerCount++;
                                            playerString+="1";
                                        } else {
                                            Player::players[i].platStandingOn = -1;
                                            playerString+="0";
                                        }
                                    }
                                    if (debug) std::cout << "[CLIENT IN] [SERVER] " << playerCount << " players [" << playerString << "]" << std::endl;
                                    break;
                                }
                                case sizeof(Client::PlayerPacket): {
                                    auto packetData = PlayerPacket(0);
                                    std::memcpy(&packetData, event.packet->data, sizeof(packetData));
                                    if (Client::id == -1 || Client::id == Player::MAX_PLAYERS) {
                                        Client::id = packetData.id;
                                        clientPacket.id = packetData.id;
                                    }
                                    Player::players[packetData.id].connected = true;
                                    Player::players[packetData.id].pos.x = packetData.x;
                                    Player::players[packetData.id].pos.y = packetData.y;
                                    Player::players[packetData.id].velocity.x = packetData.xVel;
                                    Player::players[packetData.id].velocity.y = packetData.yVel;
                                    Player::players[packetData.id].mousePos.x = packetData.mx;
                                    Player::players[packetData.id].mousePos.y = packetData.my;
                                    for (int i = 0; i < Client::KEY_COUNT; i++) {
                                        Player::players[packetData.id].keys[i] = packetData.keys[i];
                                    }
                                    if (debug) std::cout << "[CLIENT IN] [" << packetData.id << "] " << packetData.x << ", " << packetData.y << std::endl;
                                    break;
                                }
                                default: {
                                    std::cout << "[CLIENT] Who the fuck sending weird ass packets??" << std::endl;
                                    break;
                                }
                            }
                        } else if(event.channelID == CHANNEL_CHAT) {
                            Client::ChatPacket packetData = Client::ChatPacket::deserialize(reinterpret_cast<const char *>(event.packet->data), event.packet->dataLength);
                            if(packetData.ownerId == -1 && packetData.contents == "[SERVER] Pong!") {
                                pongTime = static_cast<float>(glfwGetTime());
                                int t = (int) (pongTime - pingTime) * 1000;
                                packetData.contents+=" ("+std::to_string(t)+"ms)";
                            }
                            packetData.time = static_cast<float>(glfwGetTime());
                            history.push_back(packetData);
                            if (Client::debug) std::cout << "[CLIENT IN] [CHAT] [" << packetData.ownerId << "] " << std::fixed << packetData.contents << std::endl;
                        }


                        break;
                    }
                    case ENET_EVENT_TYPE_CONNECT: {

                        printf("[CLIENT] Connected? Somehow?\n");
                    }
                    case ENET_EVENT_TYPE_DISCONNECT: {
                        printf("[CLIENT] Disconnected (somehow)\n");
                        connected = false;
                        break;
                    }
                    case ENET_EVENT_TYPE_NONE: {
                        //No packets were recieved
                        break;
                    }
                }
            }
            auto time = static_cast<float>(glfwGetTime());
            if (!packetQueue.empty()) {
                sendPacket(packetQueue[packetQueue.size() - 1]);
                packetQueue.clear();
                lastPacket = time;
            }
            if(!(*paused) && ((time - lastPacket > timeBetweenMovingPackets && (abs(clientPacket.xVel) > 0 || abs(clientPacket.yVel) > 0)) || (time - lastPacket > maxTimeBetweenPackets))) {
                sendPacket(clientPacket);
                lastPacket = time;
            }
        }
        //GAME LOOP END
        enet_peer_disconnect(peer, 0);

        while (enet_host_service(client, &event, 3000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_RECEIVE: {
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    puts("[CLIENT] Disconnection succeeded.");
                    break;
                }
                default: {
                    break;
                }
            }
        }
    }
    return EXIT_SUCCESS;
}

void Client::sendPacket(const ChatPacket& data) {
    if(Client::id == Player::MAX_PLAYERS) return;
    if (debug) std::cout << "[CLIENT OUT] [CHAT] [" << data.ownerId << "] " << data.contents << std::endl;
    std::vector<char> buffer = data.serialize();
    ENetPacket *packet = enet_packet_create(buffer.data(), buffer.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, CHANNEL_CHAT, packet);
    enet_host_flush(client);
}

void Client::sendPacket(PlayerPacket data) {
    if(Client::id == Player::MAX_PLAYERS) return;
    if (debug) std::cout << "[CLIENT OUT] [" << data.id << "] " << data.x << ", " << data.y << std::endl;
    ENetPacket *packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, CHANNEL_GAME, packet);
    enet_host_flush(client);
}

