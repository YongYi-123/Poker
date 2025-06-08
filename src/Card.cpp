#include "Card.h"
#include <stdexcept>
using namespace std;

// Constructor: initialize suit and face, compute value
Card::Card(Suit s, char f) : suit(s), face(f) {
    if (f >= '2' && f <= '9') {
        value = f - '0';
    } else {
        switch (f) {
            case 'T': case 'J': case 'Q': case 'K':
                value = 10;
                break;
            case 'A':
                value = 11;
                break;
            default:
                throw invalid_argument("Invalid face character: must be 2–9, T, J, Q, K, A");
        }
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
    return string(" ") + face;  // prepend space for alignment
}

Suit Card::getSuit() const {
    return suit;
}

string Card::getSuitStr() const {
    switch (suit) {
        case Spades:   return "Spades";
        case Hearts:   return "Hearts";
        case Diamonds: return "Diamonds";
        case Clubs:    return "Clubs";
        default:       return "Unknown";
    }
}

string Card::getSuitEmoji() const {
    switch (suit) {
        case Spades:   return "\033[1;37m♠\033[0m"; // white
        case Hearts:   return "\033[31m♥\033[0m";   // red
        case Diamonds: return "\033[31m♦\033[0m";   // red
        case Clubs:    return "\033[1;37m♣\033[0m"; // white
        default:       return "?";
    }
}