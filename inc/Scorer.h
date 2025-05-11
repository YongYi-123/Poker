#ifndef SCORER_H
#define SCORER_H

#include <vector>
#include <string>
#include "Card.h"

class Scorer {
public:
    static std::pair<std::string, int> evaluate(const std::vector<Card>& playedCards);
};

#endif
 