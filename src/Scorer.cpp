#include "Scorer.h"
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <set>

using namespace std;

ScoreResult Scorer::evaluate(const vector<Card>& playedCards) {
    if (playedCards.empty()) return {"No Cards", 0, 1, {}};

    map<char, int> faceCount;
    map<Suit, int> suitCount;
    vector<char> faces;
    vector<int> values; // from card.getValue()
    vector<int> used;

    for (const auto& card : playedCards) {
        char f = card.getFace();
        int v = card.getValue();
        faceCount[f]++;
        suitCount[card.getSuit()]++;
        faces.push_back(f);
        values.push_back(v);
    }

    // Detect flush
    bool isFlush = (playedCards.size() == 5) &&
                   all_of(playedCards.begin(), playedCards.end(),
                          [&](const Card& c) { return c.getSuit() == playedCards[0].getSuit(); });

    // Detect straight
    map<char, int> faceRank = {
        {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5},
        {'6', 6}, {'7', 7}, {'8', 8}, {'9', 9},
        {'T', 10}, {'J', 11}, {'Q', 12}, {'K', 13}, {'A', 14}
    };

    vector<int> ranks;
    for (char f : faces)
        ranks.push_back(faceRank[f]);

    sort(ranks.begin(), ranks.end());
    ranks.erase(unique(ranks.begin(), ranks.end()), ranks.end());

    bool isStraight = false;
    if (playedCards.size() == 5 && ranks.size() == 5) {
        isStraight = true;
        for (int i = 1; i < 5; ++i) {
            if (ranks[i] != ranks[i - 1] + 1) {
                isStraight = false;
                break;
            }
        }
    }

    string handType = "High Card";
    int multiplier = 1;

    if (isFlush && isStraight) {
        handType = "Straight Flush";
        multiplier = 9;
        used = values;
    }
    else if (any_of(faceCount.begin(), faceCount.end(), [](const auto& p) { return p.second == 4; })) {
        handType = "Four of a Kind";
        multiplier = 8;
        for (const auto& card : playedCards)
            if (faceCount[card.getFace()] == 4)
                used.push_back(card.getValue());
    }
    else if (faceCount.size() == 2 &&
             any_of(faceCount.begin(), faceCount.end(), [](const auto& p) { return p.second == 3; })) {
        handType = "Full House";
        multiplier = 7;
        for (const auto& card : playedCards)
            used.push_back(card.getValue());
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
    else if (any_of(faceCount.begin(), faceCount.end(), [](const auto& p) { return p.second == 3; })) {
        handType = "Three of a Kind";
        multiplier = 4;
        for (const auto& card : playedCards)
            if (faceCount[card.getFace()] == 3)
                used.push_back(card.getValue());
    }
    else {
        int pairCount = count_if(faceCount.begin(), faceCount.end(),
                                 [](const auto& p) { return p.second == 2; });

        if (pairCount == 2) {
            handType = "Two Pair";
            multiplier = 3;
            for (const auto& card : playedCards)
                if (faceCount[card.getFace()] == 2)
                    used.push_back(card.getValue());
        }
        else if (pairCount == 1) {
            handType = "Pair";
            multiplier = 2;
            for (const auto& card : playedCards)
                if (faceCount[card.getFace()] == 2)
                    used.push_back(card.getValue());
        }
        else {
            handType = "High Card";
            multiplier = 1;
            used.push_back(*max_element(values.begin(), values.end()));
        }
    }

    // 不乘 multiplier，讓 GameManager 控制倍數計算
    int score = 0;
    for (int v : used) score += v;

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