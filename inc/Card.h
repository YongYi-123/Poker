#ifndef CARD_H
#define CARD_H

#include <string>

enum Suit { Spades, Hearts, Diamonds, Clubs };

class Card {
private:
    Suit suit;
    int value;     // 2~11, where A = 11
    char face;     // '2'~'9', 'T', 'J', 'Q', 'K', 'A'
public:
    Card(Suit s, char f);
    int getValue() const;
    char getFace() const;
    std::string getFaceStr() const;
    Suit getSuit() const;
    std::string getSuitStr() const;
    std::string getSuitEmoji() const;
};


#endif