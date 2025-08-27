#include "core/GameServer.h"
#include "utils/Logger.h"
#include <thread>
#include <chrono>
#include <csignal>

volatile bool server_running = true;
// Signal handler for graceful shutdown
void signal_handler(int signal) {
    LOG_INFO("Received signal " + std::to_string(signal) + ". Shutting down server...");
    server_running = false;
}
int main() {
    // Set up signal handlers for graceful shutdown
    signal(SIGINT, signal_handler);
#ifdef SIGTERM
    signal(SIGTERM, signal_handler);
#endif
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
    
    LOG_INFO("After starting game:");
    server.listRooms();
    
    // Simulate server running
    LOG_INFO("Server is running... (Press Ctrl+C to stop)");
    
    // Initialize and start the server
    if (!server.initialize(8080)) {
        LOG_ERR("Failed to initialize server!");
        return 1;
    }
    LOG_INFO("Server is running on port 8080... (Press Ctrl+C to stop)");
    
    std::thread server_thread([&server]() {
        server.run();
    });
    while(server_running) {
        server.CleanUpRooms();
        server.SendUpdatesToClients();
        server.LogServerStats();
        server.HandleGameLogic();
        static auto last_stats = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(now - last_stats).count() >= 30) {
            LOG_INFO("Server running...");
            last_stats = now;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Wait for server thread to finish
    if (server_thread.joinable()) {
        server_thread.join();
    }
    return 0;
}