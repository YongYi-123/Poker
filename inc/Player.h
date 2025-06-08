#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "Hand.h"
#include "Item.h"

class Player {
private:
    std::string username;
    int score;
    int money;
    int nextScoreMultiplier = 1;
    Hand hand;
    std::map<std::string, int> handStats;
    std::vector<std::unique_ptr<Item>> inventory;

    // Combo Mode
    int lastMultiplier = 0;
    int comboCount = 0;

public:
    Player(const std::string& name);

    // Accessors
    std::string getUsername() const;
    int getScore() const;
    int getMoney() const;
    Hand& getHand();
    const std::vector<std::unique_ptr<Item>>& getInventory() const;
    const std::map<std::string, int>& getStats() const;
    int getNextScoreMultiplier() const;
    void setNextScoreMultiplier(int m);
    void setScore(int s);
    std::string getItemNameByIndex(int index) const;

    // Game logic
    void addScore(int amount);
    void addMoney(int amount);
    void updateStats(const std::string& handType);
    void addToInventory(std::unique_ptr<Item> item);
    void useItemByIndex(int index);
    void copyRandomCardInHand();
    void changeCardSuits(Suit suit, int count);

    // Combo logic
    void resetCombo();
    void updateCombo(int currentMultiplier);
    int getComboMultiplier() const;
    void resetStats();

    // Persistence
    bool load();
    void save();
    static bool playerExists(const std::string& name);

    // UI display
    void displayInventory() const;
    void displayStats() const;

    void drawHand(const std::vector<Card>& cards);
    bool isHandEmpty() const;
    void showHand() const;
    std::vector<int> chooseCardsToPlay() const;
    std::vector<Card> playCards(const std::vector<int>& indices);
    std::vector<int> chooseCardsToDiscard() const;
    void discardCards(const std::vector<int>& indices);
    void addCardsToHand(const std::vector<Card>& cards);
    void showStats() const;
};

#endif
