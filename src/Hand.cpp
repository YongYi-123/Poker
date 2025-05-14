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

void Hand::display() const {
    std::cout << "Current hand:\n";
    for (size_t i = 0; i < cards.size(); ++i) {
        std::cout << "[" << i << "] "
                  << cards[i].getFaceStr() << cards[i].getSuitEmoji()
                  << " (" << cards[i].getSuitStr() << ")\n";
    }
}

void Hand::setCards(const std::vector<Card>& newCards) {
    cards = newCards;
}
std::vector<Card> Hand::removeCards(const std::vector<int>& indices) {
    std::vector<int> sorted = indices;
    std::sort(sorted.rbegin(), sorted.rend());

    std::vector<Card> removed;
    for (int i : sorted) {
        if (i >= 0 && i < static_cast<int>(cards.size())) {
            removed.push_back(cards[i]);
            cards.erase(cards.begin() + i);
        }
    }
    std::reverse(removed.begin(), removed.end());  // keep original order
    return removed;
}
size_t Hand::size() const {
    return cards.size();
}
