#include "Player.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>

using json = nlohmann::json;
const std::string FILE_NAME = "players.json";

Player::Player(const std::string& name)
    : username(name), score(0), money(0) {
    load(); // Load player data if it exists
}

// Accessors
std::string Player::getUsername() const {
    return username;
}

int Player::getScore() const {
    return score;
}

int Player::getMoney() const {
    return money;
}

Hand& Player::getHand() {
    return hand;
}

const std::vector<std::string>& Player::getInventory() const {
    return inventory;
}

const std::map<std::string, int>& Player::getStats() const {
    return handStats;
}

// Game logic
void Player::addScore(int amount) {
    score += amount;
}

void Player::addMoney(int amount) {
    money += amount;
}

void Player::updateStats(const std::string& handType) {
    handStats[handType]++;
}

void Player::addToInventory(const std::string& item) {
    inventory.push_back(item);
}

void Player::useItem(const std::string& item) {
    auto it = std::find(inventory.begin(), inventory.end(), item);
    if (it != inventory.end()) {
        inventory.erase(it);
    }
}

// Persistence
void Player::load() {
    std::ifstream inFile(FILE_NAME);
    if (!inFile.is_open()) return;

    json data;
    inFile >> data;
    inFile.close();

    if (data.contains(username)) {
        json& p = data[username];
        score = p.value("score", 0);
