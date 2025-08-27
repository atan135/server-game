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

#include "GameServer.h"
#include <iostream>
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
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return false;
    }
    
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
#else
    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }
    
    // Set socket options to reuse address
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        std::cerr << "setsockopt failed" << std::endl;
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
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
#else
        std::cerr << "Bind failed" << std::endl;
#endif
        return false;
    }
    
    // Listen for connections
    if (listen(server_socket, SOMAXCONN) == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(server_socket);
        WSACleanup();
#else
        std::cerr << "Listen failed" << std::endl;
#endif
        return false;
    }
    
    max_fd = (int)server_socket;
    std::cout << "Server listening on port " << port << std::endl;
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
            std::cerr << "Select error: " << WSAGetLastError() << std::endl;
#else
            if (errno != EINTR) {
                std::cerr << "Select error" << std::endl;
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
        std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Accept failed" << std::endl;
#endif
        return;
    }
    
    char ip_str[INET_ADDRSTRLEN];
#ifdef _WIN32
    strcpy_s(ip_str, INET_ADDRSTRLEN, inet_ntoa(client_addr.sin_addr));
#else
    inet_ntop(AF_INET, &client_addr.sin_addr, ip_str, INET_ADDRSTRLEN);
#endif

    std::cout << "New connection from " << ip_str 
              << ":" << ntohs(client_addr.sin_port) << std::endl;
    
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
                std::cout << "Client disconnected" << std::endl;
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
                std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
                closesocket(client_socket);
                it = client_sockets.erase(it);
                continue;
            }
#else
            else if (valread < 0) {
                std::cerr << "Read failed: " << strerror(errno) << std::endl;
                close(client_socket);
                it = client_sockets.erase(it);
                continue;
            }
#endif
            else {
                // Process data
                std::cout << "Received: " << buffer << std::endl;
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
    
    std::cout << "Created room " << nextRoomId << ": " << roomName << std::endl;
    return room;
}

bool GameServer::deleteRoom(int roomId) {
    std::lock_guard<std::mutex> lock(roomsMutex);
    
    auto it = rooms.find(roomId);
    if (it != rooms.end()) {
        rooms.erase(it);
        std::cout << "Deleted room " << roomId << std::endl;
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
    
    std::cout << "=== Room List ===" << std::endl;
    for (const auto& pair : rooms) {
        auto room = pair.second;
        std::cout << "Room " << room->getRoomId() 
                  << " (" << room->getRoomName() << "): "
                  << room->getPlayerCount() << "/" << room->getMaxPlayers()
                  << " players" << (room->getIsStarted() ? " [IN GAME]" : "") << std::endl;
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