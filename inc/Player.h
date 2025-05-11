#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <map>
#include "Hand.h"

class Player {
private:
    std::string username;
    int score;
    int money;
    Hand hand;
    std::map<std::string, int> handStats;
    std::vector<std::string> inventory;

public:
    Player(const std::string& name);

    // Accessors
    std::string getUsername() const;
    int getScore() const;
    int getMoney() const;
    Hand& getHand();
    const std::vector<std::string>& getInventory() const;
    const std::map<std::string, int>& getStats() const;

    // Game logic
    void addScore(int amount);
    void addMoney(int amount);
    void updateStats(const std::string& handType);
    void addToInventory(const std::string& item);
    void useItem(const std::string& item);

    // Persistence
    void load();
    void save();
    static bool playerExists(const std::string& name);

    // UI display
    void displayInventory() const;
    void displayStats() const;
};

#endif
