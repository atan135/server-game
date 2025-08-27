#ifndef ROOM_H
#define ROOM_H

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

class Player {
public:
    int id;
    std::string name;
    bool isReady;
    
    Player(int playerId, const std::string& playerName) 
        : id(playerId), name(playerName), isReady(false) {}
};

class Room {
private:
    int roomId;
    std::string roomName;
    std::vector<std::shared_ptr<Player>> players;
    int maxPlayers;
    bool isStarted;
    
public:
    Room(int id, const std::string& name, int maxPlayers = 4);
    
    int getRoomId() const { return roomId; }
    std::string getRoomName() const { return roomName; }
    int getPlayerCount() const { return players.size(); }
    int getMaxPlayers() const { return maxPlayers; }
    bool getIsStarted() const { return isStarted; }
    
    bool addPlayer(std::shared_ptr<Player> player);
    bool removePlayer(int playerId);
    std::shared_ptr<Player> getPlayer(int playerId);
    std::vector<std::shared_ptr<Player>> getPlayers() const { return players; }
    
    void startGame();
    void resetRoom();
};

#endif