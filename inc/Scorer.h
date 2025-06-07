#ifndef SCORER_H
#define SCORER_H

#include <vector>
#include <string>
#include <utility>  // for std::pair
#include "Card.h"

struct ScoreResult {
    std::string handType;
    int score;
    int multiplier;
    std::vector<int> contributingValues;
};

class Scorer {
public:
    static ScoreResult evaluate(const std::vector<Card>& playedCards);
    static int getMultiplier(const std::string& handType);
};
#endif
