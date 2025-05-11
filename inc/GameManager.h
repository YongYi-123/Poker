#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Player.h"
#include "Deck.h"
#include "Scorer.h"
#include "Leaderboard.h"
#include "Shop.h"

class GameManager {
private:
    Player* currentPlayer;
    Deck deck;
    int playRoundsLeft;
    int discardRoundsLeft;
public:
    GameManager();
    void login();
    void logout();
    void mainMenu();
    void playGame();
    void playRound();
    void discardRound();
    void awardStage();
};

#endif
