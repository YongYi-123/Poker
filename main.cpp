#include <iostream>
#include <limits>
#include <thread>
#include <chrono>
#include "Display.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;
void clearScreen() {
    cout << "\033[2J\033[H";
}

void showIntroAnimation() {
    const string POKER_ART[] = {
        "██████╗  ██████╗ ██╗  ██╗███████╗██████╗ ",
        "██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝██╔══██╗",
        "██████╔╝██║   ██║█████╔╝ █████╗  ██████╔╝",
        "██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝  ██╔══██╗",
        "██║     ╚██████╔╝██║  ██╗███████╗██║  ██║",
        "╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝  ╚═╝"
    };

    clearScreen();
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j <= i; ++j)
            cout << POKER_ART[j] << endl;
        this_thread::sleep_for(chrono::milliseconds(200));
        clearScreen();
    }

    for (const auto& line : POKER_ART)
        cout << line << endl;
    this_thread::sleep_for(chrono::milliseconds(300));

    for (int i = 0; i < 2; ++i) {
        clearScreen();
        this_thread::sleep_for(chrono::milliseconds(300));
        for (const auto& line : POKER_ART)
            cout << line << endl;
        this_thread::sleep_for(chrono::milliseconds(300));
    }

    this_thread::sleep_for(chrono::seconds(3));
    clearScreen();
}

void drawWelcomeScreen() {
    clearScreen();

    cout << "╔════════════════════════════════════╗\n";
    cout << "║        Simplified Balatro Poker   ║\n";
    cout << "╚════════════════════════════════════╝\n\n";

    cout << "\033[1;37m♠ \033[31m♥ \033[31m♦ \033[1;37m♣   "
         << "\033[1;36mLet's Play Cards!\033[0m   "
         << "\033[1;37m♣ \033[31m♦ \033[31m♥ \033[1;37m♠\033[0m\n\n";

    cout << "┌─────────────┐\n";
    cout << "│   1. Login  │\n";
    cout << "└─────────────┘\n\n";

    cout << "┌─────────────┐\n";
    cout << "│   2. Exit   │\n";
    cout << "└─────────────┘\n\n";

    cout << "Enter your choice: ";
}

void showExitAnimation() {
    clearScreen();

    const vector<string> THANK = {
        "████████╗██╗  ██╗ █████╗ ███╗   ██╗██╗  ██╗",
        "╚══██╔══╝██║  ██║██╔══██╗████╗  ██║██║ ██╔╝",
        "   ██║   ███████║███████║██╔██╗ ██║█████╔╝ ",
        "   ██║   ██╔══██║██╔══██║██║╚██╗██║██╔═██╗ ",
        "   ██║   ██║  ██║██║  ██║██║ ╚████║██║  ██╗",
        "   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝"
    };

    const vector<string> FOR = {
        "███████╗ ██████╗ ██████╗ ",
        "██╔════╝██╔═══██╗██╔══██╗",
        "█████╗  ██║   ██║██████╔╝",
        "██╔══╝  ██║   ██║██╔══██╗",
        "██║     ╚██████╔╝██║  ██║",
        "╚═╝      ╚═════╝ ╚═╝  ╚═╝"
    };

    const vector<string> PLAYING = {
        "██████╗ ██╗      █████╗ ██╗   ██╗██╗███╗   ██╗ ██████╗ ",
        "██╔══██╗██║     ██╔══██╗╚██╗ ██╔╝██║████╗  ██║██╔════╝ ",
        "██████╔╝██║     ███████║ ╚████╔╝ ██║██╔██╗ ██║██║  ███╗",
        "██╔═══╝ ██║     ██╔══██║   ██║   ██║██║╚██╗██║██║   ██║",
        "██║     ███████╗██║  ██║   ██║   ██║██║ ╚████║╚██████╔╝",
        "╚═╝     ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝╚═╝  ╚═══╝ ╚═════╝ "
    };

    const vector<vector<string>> blocks = {THANK, FOR, PLAYING};
    vector<string> accumulated;

    for (const auto& block : blocks) {
        for (const auto& line : block) {
            accumulated.push_back(line);
            clearScreen();
            for (const auto& l : accumulated)
                cout << "\033[1;37m" << l << "\033[0m" << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        }
    }

    this_thread::sleep_for(chrono::seconds(2));
    clearScreen();

    cout << "\n\n\n\n\n\n          \033[1;33m:D\033[0m" << flush;
    this_thread::sleep_for(chrono::seconds(1));
    clearScreen();
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    showIntroAnimation();

    GameManager game;

    while (true) {
        drawWelcomeScreen();
        int choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {
            game.login();
            if (game.isLoggedIn()) {
                game.mainMenu();
            }
        } else if (choice == 2) {
            showExitAnimation();
            break;
        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
