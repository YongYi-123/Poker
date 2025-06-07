#include "Scorer.h"
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

ScoreResult Scorer::evaluate(const vector<Card>& playedCards) {
    if (playedCards.empty()) return {"No Cards", 0, 1, {}};

    map<int, int> valueCount;
    map<Suit, int> suitCount;
    vector<int> values;
    vector<int> used;

    for (const auto& card : playedCards) {
        int val = card.getValue();
        valueCount[val]++;
        suitCount[card.getSuit()]++;
        values.push_back(val);
    }

    sort(values.begin(), values.end());
    bool isFlush = (suitCount.size() == 1);
    bool isStraight = false;

    // Check for straight (5 unique values in sequence)
    if (values.size() >= 5) {
        set<int> uniqueVals(values.begin(), values.end());
        if (uniqueVals.size() == 5) {
            auto it = uniqueVals.begin();
            int first = *it;
            ++it;
            bool straight = true;
            for (int i = 1; i < 5; ++i, ++it) {
                if (*it != first + i) {
                    straight = false;
                    break;
                }
            }
            isStraight = straight;
        }
    }

    string handType = "High Card";
    int score = 0;
    int multiplier = 1;

    if (isFlush && isStraight) {
        handType = "Straight Flush";
        multiplier = 9;
        used = values;
    }
    else if (any_of(valueCount.begin(), valueCount.end(), [](const auto& p) { return p.second == 4; })) {
        handType = "Four of a Kind";
        multiplier = 8;
        for (const auto& [val, cnt] : valueCount) {
            if (cnt == 4) {
                used.insert(used.end(), 4, val);
            }
        }
    }
    else if (valueCount.size() == 2 &&
             any_of(valueCount.begin(), valueCount.end(), [](const auto& p) { return p.second == 3; })) {
        handType = "Full House";
        multiplier = 7;
        for (const auto& [val, cnt] : valueCount) {
            if (cnt == 3 || cnt == 2)
                used.insert(used.end(), cnt, val);
        }
    }
    else if (isFlush) {
        handType = "Flush";
        multiplier = 6;
        used = values;
    }
    else if (isStraight) {
        handType = "Straight";
        multiplier = 5;
        used = values;
    }
    else if (any_of(valueCount.begin(), valueCount.end(), [](const auto& p) { return p.second == 3; })) {
        handType = "Three of a Kind";
        multiplier = 4;
        for (const auto& [val, cnt] : valueCount) {
            if (cnt == 3)
                used.insert(used.end(), 3, val);
        }
    }
    else {
        int pairCount = 0;
        for (const auto& [val, cnt] : valueCount) {
            if (cnt == 2) pairCount++;
        }

        if (pairCount == 2) {
            handType = "Two Pair";
            multiplier = 3;
            for (const auto& [val, cnt] : valueCount) {
                if (cnt == 2)
                    used.insert(used.end(), 2, val);
            }
        }
        else if (pairCount == 1) {
            handType = "Pair";
            multiplier = 2;
            for (const auto& [val, cnt] : valueCount) {
                if (cnt == 2)
                    used.insert(used.end(), 2, val);
            }
        }
        else {
            handType = "High Card";
            multiplier = 1;
            used.push_back(*max_element(values.begin(), values.end()));
        }
    }

    // compute score from used values
    for (int v : used) score += v;
    score *= multiplier;

    return {handType, score, multiplier, used};
}
int Scorer::getMultiplier(const string& handType) {
    static const map<string, int> multipliers = {
        {"High Card", 1},
        {"Pair", 2},
        {"Two Pair", 3},
        {"Three of a Kind", 4},
        {"Straight", 5},
        {"Flush", 6},
        {"Full House", 7},
        {"Four of a Kind", 8},
        {"Straight Flush", 9}
    };

    auto it = multipliers.find(handType);
    if (it != multipliers.end()) {
        return it->second;
    }
    return 1; // Default multiplier
}