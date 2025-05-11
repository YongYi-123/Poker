#ifndef HAND_H
#define HAND_H

#include <vector>
#include "Card.h"

class Hand {
private:
    std::vector<Card> cards;
public:
    Hand();
    void addCards(const std::vector<Card>& newCards);
    std::vector<Card> getCards() const;
    void removeCards(const std::vector<int>& indices);
    void sortBySuit();
    void sortByValue();
    void display() const;
};

#endif
