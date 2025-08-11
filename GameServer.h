#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "Room.h"
#include <map>
#include <memory>
#include <mutex>

class GameServer {
private:
    std::map<int, std::shared_ptr<Room>> rooms;
    std::mutex roomsMutex;
    int nextRoomId;
    
public:
    GameServer() : nextRoomId(1) {}
    
    std::shared_ptr<Room> createRoom(const std::string& roomName, int maxPlayers = 4);
    bool deleteRoom(int roomId);
    std::shared_ptr<Room> getRoom(int roomId);
    std::vector<std::shared_ptr<Room>> getAllRooms();
    
    void listRooms();
};

#endif