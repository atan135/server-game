#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

#include "core/GameServer.h"
#include "utils/Logger.h"
#include <cstring>

#ifdef _WIN32
GameServer::~GameServer() {
    if (server_socket != INVALID_SOCKET) {
        closesocket(server_socket);
    }
    
    for (SOCKET client_socket : client_sockets) {
        if (client_socket != INVALID_SOCKET) {
            closesocket(client_socket);
        }
    }
    
    WSACleanup();
}
#endif

bool GameServer::initialize(int port) {
#ifdef _WIN32
    // Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        LOG_ERR("WSAStartup failed: " + std::to_string(result));
        return false;
    }
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        LOG_ERR("Failed to create socket: " + std::to_string(WSAGetLastError()));
        WSACleanup();
        return false;
    }
#else
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        LOG_ERR("Failed to create socket");
        return false;
    }
    
    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        LOG_ERR("setsockopt failed");
        return false;
    }
#endif

    // Bind socket
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        LOG_ERR("Bind failed: " + std::to_string(WSAGetLastError()));
        closesocket(server_socket);
        WSACleanup();
#else
        LOG_ERR("Bind failed");
#endif
        return false;
    }
    
    // Listen for connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
#ifdef _WIN32
        LOG_ERR("Listen failed: " + std::to_string(WSAGetLastError()));
        closesocket(server_socket);
        WSACleanup();
#else
        LOG_ERR("Listen failed");
#endif
        return false;
    }
    
    max_fd = (int)server_socket;
    LOG_INFO("Server listening on port " + std::to_string(port));
    return true;
}

void GameServer::run() {
    while (true) {
        // Clear the socket set
        FD_ZERO(&read_fds);
        
        // Add server socket to set
        FD_SET(server_socket, &read_fds);
        
        // Add client sockets to set
        for (auto it = client_sockets.begin(); it != client_sockets.end();) {
#ifdef _WIN32
            if (*it != INVALID_SOCKET) {
                FD_SET(*it, &read_fds);
            } else {
                it = client_sockets.erase(it);
                continue;
            }
#else
            if (*it > 0) {
                FD_SET(*it, &read_fds);
            }
#endif
            ++it;
        }
        
        // Wait for activity
        int activity = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
#ifdef _WIN32
            LOG_ERR("Select error: " + std::to_string(WSAGetLastError()));
#else
            if (errno != EINTR) {
                LOG_ERR("Select error");
            }
#endif
            break;
        }
        
        // Check for new connection
        if (FD_ISSET(server_socket, &read_fds)) {
            handle_new_connection();
        }
        
        // Check for data from clients
        handle_client_data();
    }
}

void GameServer::handle_new_connection() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    auto new_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len);
    if (new_socket == INVALID_SOCKET) {
#ifdef _WIN32
        LOG_ERR("Accept failed: " + std::to_string(WSAGetLastError()));
#else
        LOG_ERR("Accept failed");
#endif
        return;
    }
    
    char ip_str[INET_ADDRSTRLEN];
#ifdef _WIN32
    strcpy_s(ip_str, INET_ADDRSTRLEN, inet_ntoa(client_addr.sin_addr));
#else
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
#endif

    LOG_INFO("New connection from " + std::string(ip_str) + 
             ":" + std::to_string(ntohs(client_addr.sin_port)));
    
    // Add new socket to list
    client_sockets.push_back(new_socket);
    
    // Update max_fd
#ifndef _WIN32
    if ((int)new_socket > max_fd) {
        max_fd = (int)new_socket;
    }
#endif
}

void GameServer::handle_client_data() {
    for (auto it = client_sockets.begin(); it != client_sockets.end();) {
#ifdef _WIN32
        SOCKET client_socket = *it;
#else
        int client_socket = *it;
#endif
        
        if (FD_ISSET(client_socket, &read_fds)) {
            char buffer[1024] = {0};
#ifdef _WIN32
            int valread = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
#else
            ssize_t valread = read(client_socket, buffer, sizeof(buffer) - 1);
#endif
            
            if (valread == 0) {
                // Client disconnected
                LOG_INFO("Client disconnected");
#ifdef _WIN32
                closesocket(client_socket);
#else
                close(client_socket);
#endif
                it = client_sockets.erase(it);
                continue;
            } 
#ifdef _WIN32
            else if (valread == SOCKET_ERROR) {
                LOG_ERR("Recv failed: " + std::to_string(WSAGetLastError()));
                closesocket(client_socket);
                it = client_sockets.erase(it);
                continue;
            }
#else
            else if (valread < 0) {
                LOG_ERR("Read failed: " + std::string(strerror(errno)));
                close(client_socket);
                it = client_sockets.erase(it);
                continue;
            }
#endif
            else {
                // Process data
                LOG_DEBUG("Received: " + std::string(buffer));
                // Echo back for testing
#ifdef _WIN32
                send(client_socket, buffer, valread, 0);
#else
                write(client_socket, buffer, valread);
#endif
            }
        }
        ++it;
    }
}

void GameServer::broadcast_message(const std::string& message) {
    for (auto client_socket : client_sockets) {
#ifdef _WIN32
        send(client_socket, message.c_str(), message.length(), 0);
#else
        write(client_socket, message.c_str(), message.length());
#endif
    }
}

std::shared_ptr<Room> GameServer::createRoom(const std::string& roomName, int maxPlayers) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    auto room = std::make_shared<Room>(nextRoomId, roomName, maxPlayers);
    rooms[nextRoomId] = room;
    
    LOG_INFO("Created room " + std::to_string(nextRoomId) + ": " + roomName);
    return room;
}

bool GameServer::deleteRoom(int roomId) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    auto it = rooms.find(roomId);
    if (it != rooms.end()) {
        rooms.erase(it);
        LOG_INFO("Deleted room " + std::to_string(roomId));
        return true;
    }
    return false;
}

std::shared_ptr<Room> GameServer::getRoom(int roomId) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    auto it = rooms.find(roomId);
    if (it != rooms.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Room>> GameServer::getAllRooms() {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    std::vector<std::shared_ptr<Room>> roomList;
    for (const auto& pair : rooms) {
        roomList.push_back(pair.second);
    }
    return roomList;
}

void GameServer::listRooms() {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    LOG_INFO("=== Room List ===");
    for (const auto& pair : rooms) {
        auto room = pair.second;
        std::string status = room->getIsStarted() ? " [IN GAME]" : "";
        LOG_INFO("Room " + std::to_string(room->getRoomId()) + 
                 " (" + room->getRoomName() + "): " +
                 std::to_string(room->getPlayerCount()) + "/" + 
                 std::to_string(room->getMaxPlayers()) + " players" + status);
    }
}
void GameServer::CleanUpRooms(){

}
void GameServer::SendUpdatesToClients(){

}
void GameServer::HandleGameLogic(){

}
void GameServer::LogServerStats(){

}