#include <iostream>
#include "GameManager.h"
#ifdef _WIN32
#include <windows.h>  // required for SetConsoleOutputCP
#endif

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // enable UTF-8 output in Windows terminal
#endif

    std::cout << "==============================\n";
    std::cout << "   Simplified Balatro Poker   \n";
    std::cout << "==============================\n\n";

    GameManager game;
    game.login();
    game.mainMenu();

    std::cout << "\nThanks for playing! Goodbye. :D\n";
    return 0;
}
