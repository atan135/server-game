#include "core/GameServer.h"
#include "game/Room.h"
#include <iostream>

// Example of how to use the reorganized project structure
int main() {
    std::cout << "Game Server Example" << std::endl;
    
    // Create server instance
    GameServer server;
    
    // Create a game room
    auto room = server.createRoom("Example Room", 4);
    if (room) {
        std::cout << "Created room: " << room->getRoomName() << std::endl;
    }
    
    // List all rooms
    server.listRooms();
    
    return 0;
}
