#include "Player.h"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <cstdlib>  // for rand()

using json = nlohmann::json;
const std::string FILE_NAME = "players.json";

Player::Player(const std::string& name)
    : username(name), score(0), money(0), nextScoreMultiplier(1) {
    load(); // Load player data if it exists
}

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

int Player::getNextScoreMultiplier() const {
    return nextScoreMultiplier;
}

void Player::setScore(int s) {
    score = s;
}

void Player::setNextScoreMultiplier(int m) {
    nextScoreMultiplier = m;
}

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

void Player::copyRandomCardInHand() {
    auto cards = hand.getCards();
    if (!cards.empty()) {
        int randIndex = rand() % cards.size();
        Card copy = cards[randIndex];
        hand.addCards({copy});
    }
}

void Player::changeCardSuits(Suit newSuit, int count) {
    std::vector<Card> cards = hand.getCards();
    int changed = 0;
    for (auto& card : cards) {
        if (changed >= count) break;
        card = Card(newSuit, card.getFace());
        ++changed;
    }
    hand.setCards(cards);
}

// Persistence
void Player::load() {
    std::ifstream inFile(FILE_NAME);
    if (!inFile.is_open()) return;

    std::stringstream buffer;
    buffer << inFile.rdbuf();
    std::string content = buffer.str();
    inFile.close();

    if (content.empty()) return;

    json data = json::parse(content, nullptr, false);
    if (data.is_discarded()) {
        std::cerr << "Warning: Malformed players.json. Ignoring contents.\n";
        return;
    }

    if (data.contains(username) && data[username].is_object()) {
        json& p = data[username];
        score = p.value("score", 0);
        money = p.value("money", 0);
        inventory = p.value("inventory", std::vector<std::string>{});
        handStats = p.value("handStats", std::map<std::string, int>{});
    }
}

void Player::save() {
    json data;

    std::ifstream inFile(FILE_NAME);
    if (inFile.is_open()) {
        std::stringstream buffer;
        buffer << inFile.rdbuf();
        std::string content = buffer.str();
        inFile.close();

        if (!content.empty()) {
            data = json::parse(content, nullptr, false);
            if (data.is_discarded()) {
                std::cerr << "Warning: Invalid players.json format. Overwriting...\n";
                data = json::object();
            }
        }
    }

    data[username]["score"] = score;
    data[username]["money"] = money;
    data[username]["inventory"] = inventory;
    data[username]["handStats"] = handStats;

    std::ofstream outFile(FILE_NAME);
    if (!outFile) {
        std::cerr << "Error: Failed to write to " << FILE_NAME << "\n";
        return;
    }

    outFile << data.dump(4);
    outFile.close();
}

void Player::displayInventory() const {
    std::cout << "Inventory:\n";
    if (inventory.empty()) {
        std::cout << "  (empty)\n";
    } else {
        for (const auto& item : inventory) {
            std::cout << "  - " << item << "\n";
        }
    }
}

void Player::displayStats() const {
    std::cout << "Hand Stats:\n";
    if (handStats.empty()) {
        std::cout << "  (no data)\n";
    } else {
        for (const auto& pair : handStats) {
            std::cout << "  " << pair.first << ": " << pair.second << "\n";
        }
    }
}

void Player::drawHand(const std::vector<Card>& cards) {
    hand.setCards(cards);
}

bool Player::isHandEmpty() const {
    return hand.size() == 0;
}

void Player::showHand() const {
    hand.display();
}

std::vector<int> Player::chooseCardsToPlay() const {
    std::cout << "Enter indices to play (space separated): ";
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::vector<int> indices;
    int i;
    while (iss >> i) indices.push_back(i);
    return indices;
}

std::vector<Card> Player::playCards(const std::vector<int>& indices) {
    return hand.removeCards(indices);
}

std::vector<int> Player::chooseCardsToDiscard() const {
    std::cout << "Enter indices to discard (space separated): ";
    std::string line;
    std::getline(std::cin, line);
    std::istringstream iss(line);
    std::vector<int> indices;
    int i;
    while (iss >> i) indices.push_back(i);
    return indices;
}

void Player::discardCards(const std::vector<int>& indices) {
    hand.removeCards(indices);
}

void Player::addCardsToHand(const std::vector<Card>& cards) {
    hand.addCards(cards);
}

void Player::showStats() const {
    displayStats();
}
