#include "GameServer.h"
#include <iostream>

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