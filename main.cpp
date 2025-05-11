#include <iostream>
#include "headers/GameManager.h"

int main() {
    std::cout << "==============================" << std::endl;
    std::cout << "   Simplified Balatro Poker   " << std::endl;
    std::cout << "==============================\n" << std::endl;

    GameManager game;
    game.mainMenu();  // Handles login, leaderboard, game loop, etc.

    std::cout << "\nThanks for playing! Goodbye.\n";
    return 0;
}
