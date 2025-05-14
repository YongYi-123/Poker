#ifndef SCORER_H
#define SCORER_H

#include <vector>
#include <string>
#include <utility>  // for std::pair
#include "Card.h"

class Scorer {
public:
    // Evaluate the played hand and return a pair of (hand type, score)
    static std::pair<std::string, int> evaluate(const std::vector<Card>& playedCards);
};

#endif
