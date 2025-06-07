#include "Hand.h"
#include <iostream>
#include <algorithm>

int getFaceRank(char f) {
    switch (f) {
        case 'A': return 14;
        case 'K': return 13;
        case 'Q': return 12;
        case 'J': return 11;
        case 'T': return 10;
        default:  return f - '0';  // '2' to '9'
    }
}

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
            return getFaceRank(a.getFace()) > getFaceRank(b.getFace());  // sort within suit by rank
        return a.getSuit() < b.getSuit();  // group by suit
    });
}

// Sort cards by value (descending), suit as tie-breaker
void Hand::sortByValue() {
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
        int rankA = getFaceRank(a.getFace());
        int rankB = getFaceRank(b.getFace());

        if (rankA == rankB)
            return a.getSuit() < b.getSuit();  // break tie by suit
        return rankA > rankB;  // higher rank first
    });
}
void Hand::display(bool showIndices) const {
    int n = cards.size();
    int pad = 8 - n;

    if (n == 0) {
        std::cout << "║   (empty hand)\n";
        return;
    }

    // Index line (optional)
    if (showIndices) {
        std::cout << "║   ";
        for (int i = 0; i < n; ++i)
            std::cout << "  " << std::setw(2) << i << "    ";
        std::cout << std::string(pad * 8, ' ') << " ║\n";
    }

    // Top border
    std::cout << "║   ";
    for (int i = 0; i < n; ++i)
        std::cout << "┌─────┐ ";
    std::cout << std::string(pad * 8, ' ') << " ║\n";

    // Face values
    std::cout << "║   ";
    for (const auto& card : cards)
        std::cout << "│ " << std::setw(2) << card.getFaceStr() << "  │ ";
    std::cout << std::string(pad * 8, ' ') << " ║\n";

    // Suits
    std::cout << "║   ";
    for (const auto& card : cards)
        std::cout << "│  " << card.getSuitEmoji() << "  │ ";
    std::cout << std::string(pad * 8, ' ') << " ║\n";

    // Bottom border
    std::cout << "║   ";
    for (int i = 0; i < n; ++i)
        std::cout << "└─────┘ ";
    std::cout << std::string(pad * 8, ' ') << " ║\n";
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
