#include "Hand.h"
#include <iostream>
#include <algorithm>

// Constructor
Hand::Hand() {}

// Add cards to hand
void Hand::addCards(const std::vector<Card>& newCards) {
    cards.insert(cards.end(), newCards.begin(), newCards.end());
}

// Return a copy of cards
std::vector<Card> Hand::getCards() const {
    return cards;
}

// Remove cards by indices (assumed to be valid and sorted descending)
void Hand::removeCards(const std::vector<int>& indices) {
    std::vector<int> sortedIndices = indices;
    std::sort(sortedIndices.rbegin(), sortedIndices.rend()); // remove from back to front
    for (int index : sortedIndices) {
        if (index >= 0 && index < static_cast<int>(cards.size())) {
            cards.erase(cards.begin() + index);
        }
    }
}

// Sort cards by suit (Spades > Hearts > Diamonds > Clubs)
void Hand::sortBySuit() {
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        if (a.getSuit() == b.getSuit())
            return a.getValue() > b.getValue(); // descending value
        return a.getSuit() < b.getSuit(); // suit enum order
    });
}

// Sort cards by value (descending), suit as tie-breaker
void Hand::sortByValue() {
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        if (a.getValue() == b.getValue())
            return a.getSuit() < b.getSuit(); // tie-breaker by suit
        return a.getValue() > b.getValue(); // descending value
    });
}

// Display cards to console
void Hand::display() const {
    std::cout << "Current hand:\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << "[" << i << "] " << cards[i].toString() << "\n";
    }
}
