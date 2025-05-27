#ifndef HAND_H
#define HAND_H

#include <vector>
#include <iomanip>
#include "Card.h"

class Hand {
private:
    std::vector<Card> cards;
public:
    Hand();
    void addCards(const std::vector<Card>& newCards);
    std::vector<Card> getCards() const;
    void sortBySuit();
    void sortByValue();
    void display(bool showIndices = true) const;
    void setCards(const std::vector<Card>& newCards);
    std::vector<Card> removeCards(const std::vector<int>& indices);  // Overload
    size_t size() const;

};

#endif
 