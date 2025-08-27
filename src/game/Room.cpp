#include "game/Room.h"
#include <iostream>

Room::Room(int id, const std::string& name, int maxPlayers) 
    : roomId(id), roomName(name), maxPlayers(maxPlayers), isStarted(false) {}

bool Room::addPlayer(std::shared_ptr<Player> player) {
    if (players.size() >= maxPlayers || isStarted) {
        return false;
    }
    
    // Check if player already exists
    for (const auto& p : players) {
        if (p->id == player->id) {
            return false;
        }
    }
    
    players.push_back(player);
    return true;
}

bool Room::removePlayer(int playerId) {
    auto it = std::find_if(players.begin(), players.end(),
        [playerId](const std::shared_ptr<Player>& p) {
            return p->id == playerId;
        });
        
    if (it != players.end()) {
        players.erase(it);
        return true;
    }
    return false;
}

std::shared_ptr<Player> Room::getPlayer(int playerId) {
    for (const auto& player : players) {
        if (player->id == playerId) {
            return player;
        }
    }
    return nullptr;
}

void Room::startGame() {
    if (players.size() >= 2 && !isStarted) {
        isStarted = true;
        // Game logic would go here
        std::cout << "Game started in room " << roomId << std::endl;
    }
}

void Room::resetRoom() {
    isStarted = false;
    for (auto& player : players) {
        player->isReady = false;
    }
}