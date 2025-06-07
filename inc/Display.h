#ifndef DISPLAY_H
#define DISPLAY_H

#include "Player.h"
#include "Card.h"
#include <vector>
#include <string>

// For input phase — shows current hand only
void drawHandOnlyInterface(const Player& player,
                           const std::vector<Card>& handCards,
                           int playRoundsLeft,
                           int discardRoundsLeft);

// For result phase — shows played/discarded cards
void drawResultInterface(
    const Player& player,
    const std::vector<Card>& played,
    const std::vector<Card>& remaining,
    const std::vector<int>& playedIndices,
    int playRoundsLeft,
    int discardRoundsLeft,
    const std::string& handType,
    int handScore,
    int baseMultiplier,
    int itemMultiplier,
    const std::vector<int>& contributingValues
);


                         
void drawAwardScreen(const Player& player, int finalScore, int bestScore);

#endif
