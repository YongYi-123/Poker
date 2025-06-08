#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <memory>
#include "Card.h"

class Player;  // 👈 Forward declare

class Item {
public:
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual int getPrice() const = 0;
    virtual void apply(Player& player) const = 0;
    virtual ~Item() {}

    static std::unique_ptr<Item> fromName(const std::string& name);
};

class ScoreDoubleItem : public Item {
public:
    std::string getName() const override { return "Score Double Ticket"; }
    std::string getDescription() const override { return "Doubles the score of the next played hand"; }
    int getPrice() const override { return 10; }
    void apply(Player& player) const override;
};

class SuitChangeItem : public Item {
private:
    Suit targetSuit;
    std::string name;
public:
    SuitChangeItem(Suit suit, const std::string& itemName)
        : targetSuit(suit), name(itemName) {}

    std::string getName() const override { return name; }
    std::string getDescription() const override {
        return "Changes 3 random cards in hand to " + name.substr(0, name.find(" Ticket"));
    }
    int getPrice() const override { return 5; }
    void apply(Player& player) const override;
};

class CopyCardItem : public Item {
public:
    std::string getName() const override { return "Copy Ticket"; }
    std::string getDescription() const override { return "Copies a randomly selected card in hand"; }
    int getPrice() const override { return 8; }
    void apply(Player& player) const override;
};

#endif
