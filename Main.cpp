#include "GameServer.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    GameServer server;
    
    // Create some sample rooms
    auto room1 = server.createRoom("Battle Room", 4);
    auto room2 = server.createRoom("Casual Game", 2);
    
    // Create some players
    auto player1 = std::make_shared<Player>(1, "Alice");
    auto player2 = std::make_shared<Player>(2, "Bob");
    auto player3 = std::make_shared<Player>(3, "Charlie");
    
    // Add players to rooms
    room1->addPlayer(player1);
    room1->addPlayer(player2);
    
    room2->addPlayer(player3);
    
    // List all rooms
    server.listRooms();
    
    // Start a game in room1
    room1->startGame();
    
    std::cout << "\nAfter starting game:" << std::endl;
    server.listRooms();
    
    // Simulate server running
    std::cout << "\nServer is running... (Press Ctrl+C to stop)" << std::endl;
    
    // In a real server, you would have a network loop here
    // For this example, we'll just sleep
    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(10));
    }
    
    return 0;
}