#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "Room.h"
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <string>
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>
    #include <errno.h>
#endif
#ifdef _WIN32
    typedef int socklen_t;
    //typedef int ssize_t;
#else
    const int INVALID_SOCKET = -1;
    const int SOCKET_ERROR = -1;
#endif

class GameServer {
private:
    std::map<int, std::shared_ptr<Room>> rooms;
    std::mutex roomsMutex;
    int nextRoomId;
    
    #ifdef _WIN32
        SOCKET server_socket;
        std::vector<SOCKET> client_sockets;
    #else
        int server_socket;
        std::vector<int> client_sockets;
    #endif
    int max_fd;
    fd_set read_fds;
public:
    GameServer() : nextRoomId(1) , server_socket(INVALID_SOCKET){}
    
    std::shared_ptr<Room> createRoom(const std::string& roomName, int maxPlayers = 4);
    bool deleteRoom(int roomId);
    std::shared_ptr<Room> getRoom(int roomId);
    std::vector<std::shared_ptr<Room>> getAllRooms();
    void listRooms();

    bool initialize(int port);
    void run();
    void handle_new_connection();
    void handle_client_data();
    void broadcast_message(const std::string& message);

    void CleanUpRooms();
    void SendUpdatesToClients();
    void HandleGameLogic();
    void LogServerStats();

    #ifdef _WIN32
        ~GameServer();
    #endif
};

#endif