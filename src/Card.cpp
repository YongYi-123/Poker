#include "Card.h"
#include <stdexcept>
using namespace std;
// Constructor to initialize the card based on face and suit
Card::Card(Suit s, char f) : suit(s), face(f) {
    switch (f) {
        case '2' ... '9':
            value = f - '0';
            break;
        case 'T':
        case 'J':
        case 'Q':
        case 'K':
            value = 10;
            break;
        case 'A':
            value = 11;
            break;
        default:
            throw std::invalid_argument("Invalid face character");
    }
}

int Card::getValue() const {
    return value;
}

char Card::getFace() const {
    return face;
}

std::string Card::getFaceStr() const {
    if (face == 'T') return "10";
    return std::string(" ") + face;         
}

Suit Card::getSuit() const {
    return suit;
}

string Card::getSuitStr() const {
    switch (suit) {
        case Spades: return "Spades";
        case Hearts: return "Hearts";
        case Diamonds: return "Diamonds";
        case Clubs: return "Clubs";
        default: return "Unknown";
    }
}

string Card::getSuitEmoji() const {
    switch (suit) {
        case Spades: return u8"♠";
        case Hearts: return u8"♥";
        case Diamonds: return u8"♦";
        case Clubs: return u8"♣";
        default: return "?";
    }
}
