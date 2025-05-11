#ifndef DECK_H
#define DECK_H

#include <vector>
#include "Card.h"

class Deck {
private:
    std::vector<Card> cards;
public:
    Deck();
    void shuffle();
    std::vector<Card> deal(int n);
    void reset();
};

#endif
 