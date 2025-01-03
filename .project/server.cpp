//
// Created by cobble on 12/28/2024.
//

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include "server.h"

#ifdef _WIN32

#include <winsock2.h>  // For Windows
#include <ws2tcpip.h>  // For Windows
#include <thread>
#include <chrono>

#pragma comment(lib, "ws2_32.lib")  // Link to Winsock library
#else
#include <unistd.h>  // For Linux/macOS
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>  // For Linux/macOS
#endif


std::vector<Client::PlayerPacket> Server::packetQueue;
ENetPeer *Server::clients[Player::MAX_PLAYERS];
int Server::playerCount = 0;
bool Server::startServer = false;
bool Server::isLan = false;

[[maybe_unused]] std::string getLocalIPAddress() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) == -1) {
        perror("gethostname");
        return "127.0.0.1"; // Fallback to localhost
    }

    struct hostent *host = gethostbyname(hostname);
    if (host == nullptr) {
        perror("gethostbyname");
        return "127.0.0.1"; // Fallback to localhost
    }

    auto **addr_list = (struct in_addr **) host->h_addr_list;
    if (addr_list[0] != nullptr) {
        return inet_ntoa(*addr_list[0]);
    }

    return "127.0.0.1"; // Fallback to localhost
}

[[maybe_unused]] std::string getPublicIPAddress() {
    // Initialize Winsock (only for Windows)
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return "";
    }
#endif

    // Create a socket to connect to the API
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation failed\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return "";
    }

    // Resolve the server address
    struct sockaddr_in server{};
    struct hostent *host = gethostbyname("api.ipify.org"); // IP address service host
    if (host == nullptr) {
        std::cerr << "Could not resolve host\n";
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return "";
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(80);  // HTTP port
    // Replace bcopy with memcpy
    memcpy((char *) &server.sin_addr.s_addr, (char *) host->h_addr, host->h_length);

    // Connect to the server
    if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        std::cerr << "Connection failed\n";
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return "";
    }

    // Send HTTP request to get public IP
    const std::string request = "GET / HTTP/1.1\r\nHost: api.ipify.org\r\nConnection: close\r\n\r\n";
    if (send(sock, request.c_str(), request.size(), 0) < 0) {
        std::cerr << "Send failed\n";
#ifdef _WIN32
        closesocket(sock);
        WSACleanup();
#else
        close(sock);
#endif
        return "";
    }

    // Receive response
    char buffer[512];
    std::string response;
    int bytesReceived;
    while ((bytesReceived = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        response.append(buffer, bytesReceived);
    }

    // Close the socket
#ifdef _WIN32
    closesocket(sock);
    WSACleanup();
#else
    close(sock);
#endif

    // Extract the public IP from the response (after the headers)
    std::string ip = response.substr(response.find("\r\n\r\n") + 4);
    return ip;
}

[[noreturn]] int Server::run() {

    if (enet_initialize() != 0) {
        fprintf(stderr, "[SERVER] An error occurred while initializing ENet!\n");
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    ENetAddress address;
    ENetHost *server;
    ENetEvent event;

    while(true) {
        while(!startServer) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        Server::startServer = false;
        for (auto &client: clients) {
            client = nullptr;
        }

        //Client::ip = "10.0.0.15";
        //Client::ip = "127.0.0.1";
        //Client::ip = getLocalIPAddress();
        Client::ip = isLan ? getLocalIPAddress() : getPublicIPAddress();

        //address.host = ENET_HOST_ANY;
        enet_address_set_host(&address, Client::ip.c_str());
        address.port = 881;

        //32 is max # of connections
        //2 is channel count
        //0s are infinite max bandwidth
        server = enet_host_create(&address, Player::MAX_PLAYERS, 2, 0, 0);
        if (server == nullptr) {
            fprintf(stderr, "[CLIENT] A server is already up! Attempting to join...\n");
            auto chatPacket = Client::ChatPacket();
            chatPacket.time = static_cast<float>(glfwGetTime());
            chatPacket.contents = "[SYSTEM] Failed to create server with ip: " + Client::ip;
            chatPacket.ownerId = Player::MAX_PLAYERS;
            Client::history.push_back(chatPacket);
            continue;
        }
        printf("[SERVER] Creating a server with ip: %s:%i\n",
               Client::ip.c_str(),
               address.port);

        auto chatPacket = Client::ChatPacket();
        chatPacket.time = static_cast<float>(glfwGetTime());
        chatPacket.contents = "[SYSTEM] Creating server with ip: " + Client::ip;
        chatPacket.ownerId = Player::MAX_PLAYERS;
        Client::history.push_back(chatPacket);

        Client::hostClient = true;
        //GAME LOOP START
        bool running = true;
        while (running) {
            while (enet_host_service(server, &event, 15) > 0) { //two while loops because possibly multiple packets in same frame

                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT: {
                        printf("[SERVER] A new client connected from %x:%u.\n",
                               event.peer->address.host,
                               event.peer->address.port);

                        if (playerCount < Player::MAX_PLAYERS) {
                            int newPlayerId = -1;
                            for (int i = 0; i < Player::MAX_PLAYERS; i++) {
                                if (clients[i] != nullptr) continue;
                                newPlayerId = i;
                                clients[newPlayerId] = event.peer;
                                sendPacket(clients[newPlayerId], Client::PlayerPacket(newPlayerId));
                                break;
                            }
                            if (newPlayerId == -1) break;
                            playerCount++;
                            for (int i = 0; i < Player::MAX_PLAYERS; i++) {
                                if (clients[i] != nullptr) {
                                    if (i == newPlayerId) { //new player sends everyone else their starting data
                                        //auto playerPacket = Client::PlayerPacket(newPlayerId);
                                        auto serverPacket = Client::ServerPacket();
                                        for(int j=0;j<Player::MAX_PLAYERS;j++) {
                                            serverPacket.players[j] = clients[j] != nullptr;
                                        }
                                        for (int j = 0; j < Player::MAX_PLAYERS; j++) {
                                            if (clients[j] != nullptr) {
                                                serverPacket.id = j;
                                                sendPacket(clients[j], serverPacket);
                                            }
                                        }
                                    } else { //everyone else sends new player their existing data
                                        auto playerPacket = Client::PlayerPacket(i);
                                        playerPacket.x = Player::players[i].pos.x;
                                        playerPacket.y = Player::players[i].pos.y;
                                        playerPacket.xVel = Player::players[i].velocity.x;
                                        playerPacket.yVel = Player::players[i].velocity.y;
                                        playerPacket.mx = Player::players[i].mousePos.x;
                                        playerPacket.my = Player::players[i].mousePos.y;
                                        for (int j = 0; j < Client::KEY_COUNT; j++) {
                                            playerPacket.keys[j] = Player::players[i].keys[j];
                                        }
                                        sendPacket(clients[newPlayerId], playerPacket);
                                    }
                                }
                            }
                        } else {
                            enet_peer_disconnect(event.peer, 0);
                        }

                        break;
                    }
                    case ENET_EVENT_TYPE_RECEIVE: {
                        if (Client::debug) printf("[SERVER] A packet of length %zu containing %s was received from %x:%u on channel %u.\n",
                                                  event.packet->dataLength,
                                                  event.packet->data,
                                                  event.peer->address.host,
                                                  event.peer->address.port,
                                                  event.channelID);

                        //game channel
                        if (event.channelID == CHANNEL_GAME) {
                            switch (event.packet->dataLength) {
                                case sizeof(Client::PlayerPacket): {
                                    auto packetData = Client::PlayerPacket(0);
                                    std::memcpy(&packetData, event.packet->data, sizeof(packetData)); //copies the actual packet data into the PacketData type
                                    if (Client::debug) std::cout << "[SERVER IN] [" << packetData.id << "] " << packetData.x << ", " << packetData.y << std::endl;
                                    packetQueue.push_back(packetData);
                                    break;
                                }
                                case sizeof(Client::ServerPacket): {

                                    break;
                                }
                                default: {
                                    if (Client::debug) std::cout << "[SERVER] Who the fuck sending weird ass packets??" << std::endl;
                                    break;
                                }
                            }
                            enet_packet_destroy(event.packet);

                            //chat channel
                        } else if (event.channelID == CHANNEL_CHAT) {
                            Client::ChatPacket packetData = Client::ChatPacket::deserialize(reinterpret_cast<const char *>(event.packet->data), event.packet->dataLength);
                            if (Client::debug) std::cout << "[SERVER IN] [CHAT] [" << packetData.ownerId << "] " << packetData.contents << std::endl;
                            if(packetData.contents == "/ping") {
                                int ownerId = packetData.ownerId;
                                packetData.ownerId = -1;
                                packetData.contents = "[SERVER] Pong!";
                                packetData.time = static_cast<float>(glfwGetTime());
                                sendPacket(clients[ownerId], packetData);
                                break;
                            }
                            for (int i = 0; i < Player::MAX_PLAYERS; i++) {
                                if (clients[i] != nullptr && Player::players[i].id != packetData.ownerId) sendPacket(clients[i], packetData);
                            }
                        }
                        break;
                    }
                    case ENET_EVENT_TYPE_DISCONNECT: {
                        printf("[SERVER] %x:%u disconnected.\n",
                               event.peer->address.host,
                               event.peer->address.port);
                        playerCount--;
                        if (playerCount == 0) running = false;
                        playerCount = 0;
                        auto serverPacket = Client::ServerPacket();
                        for(int i=0;i<Player::MAX_PLAYERS;i++) {
                            serverPacket.players[i] = clients[i] != nullptr;
                            if(clients[i] == nullptr) continue;
                            playerCount++;
                            if(clients[i]->address.host == event.peer->address.host && clients[i]->address.port == event.peer->address.port) {
                                clients[i] = nullptr;
                                serverPacket.players[i] = false;
                                continue;
                            }
                        }
                        for(int i=0;i<Player::MAX_PLAYERS;i++) {
                            if(clients[i] == nullptr) continue;
                            serverPacket.id = i;
                            sendPacket(clients[i], serverPacket);
                        }

                        break;
                    }
                    case ENET_EVENT_TYPE_NONE: {
                        break;
                    }
                }

            }
            if (playerCount != 0) {
                for (int i = 0; i < Player::MAX_PLAYERS; i++) {
                    if (clients[i] == nullptr) continue;
                    for (const auto &packet: packetQueue) {
                        if (packet.id != i) sendPacket(clients[i], packet);
                    }
                }
                packetQueue.clear();
            }
        }
        //GAME LOOP END
        enet_host_destroy(server);
    }

    return EXIT_SUCCESS;
}

void Server::sendPacket(ENetPeer *peer, const Client::ChatPacket& data) {
    if (Client::debug) std::cout << "[SERVER OUT] [CHAT] [" << data.ownerId << "] " << data.contents << std::endl;
    std::vector<char> buffer = data.serialize();
    ENetPacket *packet = enet_packet_create(buffer.data(), buffer.size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, CHANNEL_CHAT, packet); //1 is the channel
}

void Server::sendPacket(ENetPeer *peer, Client::PlayerPacket data) {
    if (Client::debug) std::cout << "[SERVER OUT] [" << data.id << "] " << data.x << ", " << data.y << std::endl;
    ENetPacket *packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, CHANNEL_GAME, packet); //0 is the channel
}

void Server::sendPacket(ENetPeer *peer, Client::ServerPacket data) {
    if (Client::debug) {
        std::string playerString;
        int pc = 0; //playerCount
        for (bool player: data.players) {
            if (player) {
                playerString += "1";
                pc++;
            } else {
                playerString += "0";
            }
        }
        std::cout << "[SERVER OUT] [SERVER] " << pc << " players [" << playerString << "]" << std::endl;
    }
    ENetPacket *packet = enet_packet_create(&data, sizeof(data), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, CHANNEL_GAME, packet); //0 is the channel
}