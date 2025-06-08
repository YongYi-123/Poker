#include "Item.h"
#include "Player.h" 
#include <iostream>

void ScoreDoubleItem::apply(Player& player) const {
    player.setNextScoreMultiplier(2);
    std::cout << "Next score will be doubled!\n";
}

void SuitChangeItem::apply(Player& player) const {
    player.changeCardSuits(targetSuit, 3);
    std::cout << "3 cards changed to " << name.substr(0, name.find(" Ticket")) << "!\n";
}

void CopyCardItem::apply(Player& player) const {
    if (player.getHand().getCards().size() >= 8) {
        std::cout << "Cannot copy card. Hand is already full (8 cards).\n";
        return;
    }

    player.copyRandomCardInHand();
    std::cout << "A card has been copied!\n";
}


std::unique_ptr<Item> Item::fromName(const std::string& name) {
    if (name == "Score Double Ticket") return std::make_unique<ScoreDoubleItem>();
    if (name == "Copy Ticket") return std::make_unique<CopyCardItem>();
    if (name == "Spade Ticket") return std::make_unique<SuitChangeItem>(Spades, name);
    if (name == "Heart Ticket") return std::make_unique<SuitChangeItem>(Hearts, name);
    if (name == "Diamond Ticket") return std::make_unique<SuitChangeItem>(Diamonds, name);
    if (name == "Club Ticket") return std::make_unique<SuitChangeItem>(Clubs, name);
    return nullptr;
}