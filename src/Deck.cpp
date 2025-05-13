#include "Deck.h"
#include <algorithm>  // for std::shuffle
#include <random>     // for std::default_random_engine
#include <ctime>      // for std::time

// Constructor: builds and shuffles the full deck
Deck::Deck() {
    reset();
}

// Reset deck to full 52 cards
void Deck::reset() {
    cards.clear();
    const std::vector<char> faces = {'2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K', 'A'};
    for (int s = Spades; s <= Clubs; ++s) {
        for (char f : faces) {
            cards.emplace_back(static_cast<Suit>(s), f);
        }
    }
}

// Shuffle the deck with time-based seed
void Deck::shuffle() {
    std::default_random_engine rng(static_cast<unsigned>(std::time(0)));
    std::shuffle(cards.begin(), cards.end(), rng);
}

// Deal n cards from the top of the deck
std::vector<Card> Deck::deal(int n) {
    std::vector<Card> hand;
    if (n > static_cast<int>(cards.size()))
        n = cards.size(); // Deal only what's available

    hand.insert(hand.end(), cards.begin(), cards.begin() + n);
    cards.erase(cards.begin(), cards.begin() + n);
    return hand;
}
