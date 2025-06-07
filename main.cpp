#include <iostream>
#include <limits>
#include "GameManager.h"

#ifdef _WIN32
#include <windows.h>  // required for SetConsoleOutputCP
#endif

using namespace std;

void drawWelcomeScreen() {
    cout << "\033[H\033[J";  // Clear screen (works on most terminals)
    cout << "==============================\n";
    cout << "   Simplified Balatro Poker   \n";
    cout << "==============================\n\n";
    cout << "♠ ♥ ♦ ♣  Let's Play Cards!  ♣ ♦ ♥ ♠\n\n";
    cout << "1. Login\n";
    cout << "2. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // enable UTF-8 output in Windows terminal
#endif

    GameManager game;

    while (true) {
        drawWelcomeScreen();
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            game.login();
            if (game.isLoggedIn()) {
                game.mainMenu();  // logout() is handled inside this
            }
        } else if (choice == 2) {
            cout << "\nThanks for playing! Goodbye. :D\n";
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
