#include "Scorer.h"
#include <map>
#include <algorithm>
#include <set>

std::pair<std::string, int> Scorer::evaluate(const std::vector<Card>& playedCards) {
    if (playedCards.empty()) return {"No Cards", 0};

    std::map<int, int> valueCount;
    std::map<Suit, int> suitCount;
    std::vector<int> values;

    for (const auto& card : playedCards) {
        int val = card.getValue();
        valueCount[val]++;
        suitCount[card.getSuit()]++;
        values.push_back(val);
    }

    std::sort(values.begin(), values.end());
    bool isFlush = (suitCount.size() == 1);
    bool isStraight = false;

    // Check for straight (need at least 5 cards)
    if (values.size() == 5) {
        isStraight = true;
        for (int i = 1; i < 5; ++i) {
            if (values[i] != values[i - 1] + 1) {
                isStraight = false;
                break;
            }
        }
    }

    // Determine hand type and score
    int multiplier = 1;
    std::string handType = "High Card";

    if (isStraight && isFlush) {
        multiplier = 9;
        handType = "Straight Flush";
    } else if (valueCount.size() == 2) {
        if (std::any_of(valueCount.begin(), valueCount.end(), [](auto& p) { return p.second == 4; })) {
            multiplier = 8;
            handType = "Four of a Kind";
        } else {
            multiplier = 7;
            handType = "Full House";
        }
    } else if (isFlush) {
        multiplier = 6;
        handType = "Flush";
    } else if (isStraight) {
        multiplier = 5;
        handType = "Straight";
    } else if (std::any_of(valueCount.begin(), valueCount.end(), [](auto& p) { return p.second == 3; })) {
        multiplier = 4;
        handType = "Three of a Kind";
    } else {
        int pairCount = 0;
        for (const auto& p : valueCount) {
            if (p.second == 2) pairCount++;
        }
        if (pairCount == 2) {
            multiplier = 3;
            handType = "Two Pair";
        } else if (pairCount == 1) {
            multiplier = 2;
            handType = "Pair";
        }
    }

    int score = 0;

    if (multiplier == 1) {
        // High Card: only highest card counts
        score = *std::max_element(values.begin(), values.end()) * multiplier;
    } else {
        for (const auto& [val, count] : valueCount) {
            if ((multiplier == 2 && count == 2) ||
                (multiplier == 3 && count == 2) ||
                (multiplier == 4 && count == 3) ||
                (multiplier == 7 && (count == 3 || count == 2)) ||
                (multiplier == 8 && count == 4) ||
                (multiplier == 9 || multiplier == 6 || multiplier == 5)) {
                score += val * count;
            }
        }
        score *= multiplier;
    }

    return {handType, score};
}
