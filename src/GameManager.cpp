#include "GameManager.h"
#include "Leaderboard.h"
#include "Scorer.h"
#include "Shop.h"
#include "Display.h"
#include <iostream>
#include <limits>
#include <sstream>
#include <algorithm>
#include <regex> 

using namespace std;

bool isAsciiPrintable(const std::string& s) {
    for (unsigned char c : s) {
       if (c < 32 || c > 126) return false;
    }
    return true;
}


GameManager::GameManager() {
    currentPlayer = nullptr;
    playRoundsLeft = 4;
    discardRoundsLeft = 3;
}

bool GameManager::isLoggedIn() const {
    return currentPlayer != nullptr;
}

void GameManager::login() {
    string name;
    while (true) {
        cout << "Enter player name to login: ";
        getline(cin, name);

        bool allSpaces = std::all_of(name.begin(), name.end(), [](unsigned char c) {
            return std::isspace(c);
        });

        if (!name.empty() && !allSpaces && isAsciiPrintable(name)) {
            break;
        } else {
            cout << "Invalid input. Please use only English letters, numbers, and symbols.\n";
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    currentPlayer = new Player(name);
    bool isReturning = currentPlayer->load();

    if (isReturning) {
        cout << "Welcome back, " << currentPlayer->getUsername() << "!\n";
        cout << "Your current balance is: $" << currentPlayer->getMoney() << "\n";
    } else {
        cout << "Welcome, new player " << currentPlayer->getUsername() << "!\n";
    }

    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


void GameManager::logout() {
    if (currentPlayer) {
        currentPlayer->save();
        delete currentPlayer;
        currentPlayer = nullptr;
        cout << "You have been logged out.\n";
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void GameManager::mainMenu() {
    int choice;
    do {
        cout << "\033[H\033[J";  // Clear screen (works on most terminals)
        cout << "==============================\n";
        cout << "   Simplified Balatro Poker   \n";
        cout << "==============================\n";
        cout << "Player: " << currentPlayer->getUsername() << "\n";
        cout << "\n=== Main Menu ===\n";
        cout << "1. Play Game\n";
        cout << "2. View Leaderboard\n";
        cout << "3. Shop\n";
        cout << "4. Logout\n";
        cout << "5. View Inventory\n";
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();  // clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // discard bad input
            cout << "Invalid input. Please enter a number.\n";
            continue;  // restart the loop
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');  // clean up input buffer

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                Leaderboard::displayTopPlayers(10, true);
                cout << "\nPress Enter to return to menu...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 3:
                Shop::purchase(*currentPlayer);
                cout << "\nPress Enter to return to menu...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            case 4:
                logout();
                break;
           case 5:
                cout << "\n=== Inventory ===\n";
                cout << "Balance: $" << currentPlayer->getMoney() << "\n";
                currentPlayer->displayInventory();
                cout << "\nPress Enter to return to menu...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                break;
            default:
                cout << "Invalid option.\n";
                cout << "\nPress Enter to return to menu...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (currentPlayer);
}

void GameManager::playGame() {
    currentPlayer->setScore(0);
    currentPlayer->resetStats();
    deck.reset();
    deck.shuffle();
    currentPlayer->drawHand(deck.deal(8));
    playRoundsLeft = 4;
    discardRoundsLeft = 3;

    while (playRoundsLeft > 0 && !currentPlayer->isHandEmpty()) {
        playRound();

        if (currentPlayer->isHandEmpty()) {
            cout << "No cards remaining. Ending game early.\n";
            break;
        }

        if (discardRoundsLeft > 0) {
            cout << "\nPress Enter to continue to discard round...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            discardRound();

            if (currentPlayer->isHandEmpty()) {
                cout << "No cards remaining after discard. Ending game early.\n";
                break;
            }
        }
    }

    cout << "\nPress Enter to continue to award stage...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    awardStage();
}

void GameManager::playRound() {
    vector<Card> selected;
    vector<int> indices;

    // === Allow item usage ===
    while (true) {
        drawHandOnlyInterface(
            *currentPlayer,
            currentPlayer->getHand().getCards(),
            playRoundsLeft,
            discardRoundsLeft
        );

        cout << "\n--- Play Round (" << playRoundsLeft << " left) ---\n";
        cout << "\nYou can:\n";
        cout << "- Type card indices to play (e.g. `0 1 3`)\n";
        cout << "- Type `sort suit` or `sort value`\n";
        cout << "- Type `inventory` to view and use an item\n";
        cout << "Enter command or card indices: ";

        string inputLine;
        getline(cin, inputLine);
        istringstream iss(inputLine);

        string firstWord;
        iss >> firstWord;

        if (firstWord == "sort") {
            string mode;
            iss >> mode;
            if (mode == "suit") currentPlayer->getHand().sortBySuit();
            else if (mode == "value") currentPlayer->getHand().sortByValue();
            else cout << "Unknown sort mode.\n";
        } 
        else if (firstWord == "inventory") {
            currentPlayer->displayInventory();

            const auto& inv = currentPlayer->getInventory();
            if (inv.empty()) {
                cout << "\nPress Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            cout << "\nEnter item index to use or press Enter to cancel: ";
            string itemInput;
            getline(cin, itemInput);

            if (!itemInput.empty()) {
                try {
                    int index = stoi(itemInput);
                    string itemName = currentPlayer->getItemNameByIndex(index);
                    if (!itemName.empty()) {
                        currentPlayer->useItemEffect(itemName);
                    } else {
                        cout << "Invalid index.\n";
                    }
                } catch (...) {
                    cout << "Invalid input.\n";
                }
            }

            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            try {
                int index = stoi(firstWord);
                indices.push_back(index);
                int next;
                while (iss >> next) indices.push_back(next);
                if (indices.size() > 5) {
                    cout << "You can only play up to 5 cards. Try again.\n";
                    indices.clear();
                    cout << "\nPress Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }
                selected = currentPlayer->playCards(indices);

                if (selected.empty()) {
                    cout << "No valid cards selected.\n";
                    indices.clear();
                    cout << "\nPress Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');    
                    continue;
                }
                break;
            } catch (...) {
                cout << "Invalid input. Try again.\n";
                cout << "\nPress Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                indices.clear();
            }
        }
    }
    
    ScoreResult result = Scorer::evaluate(selected);
    int score = result.score;
    int baseMultiplier = result.multiplier;
    int itemMultiplier = currentPlayer->getNextScoreMultiplier();

    if (itemMultiplier > 1) {
        score *= itemMultiplier;
    }

    currentPlayer->addScore(score);
    currentPlayer->updateStats(result.handType);
    playRoundsLeft--;

    drawResultInterface(
        *currentPlayer,
        selected,
        currentPlayer->getHand().getCards(),
        indices,
        playRoundsLeft,
        discardRoundsLeft,
        result.handType,
        score,
        baseMultiplier,
        itemMultiplier,
        result.contributingValues 
    );

    currentPlayer->setNextScoreMultiplier(1);
}

void GameManager::discardRound() {
    vector<int> indices;

    while (true) {
        drawHandOnlyInterface(
            *currentPlayer,
            currentPlayer->getHand().getCards(),
            playRoundsLeft,
            discardRoundsLeft
        );

        cout << "\n--- Discard Round (" << discardRoundsLeft << " left) ---\n";
        cout << "\nYou can:\n";
        cout << "- Type card indices to discard (e.g. `0 1 3`)\n";
        cout << "- Type `sort suit` or `sort value`\n";
        cout << "Enter command or card indices: ";

        string inputLine;
        getline(cin, inputLine);
        istringstream iss(inputLine);

        string firstWord;
        iss >> firstWord;

        if (firstWord == "sort") {
            string mode;
            iss >> mode;
            if (mode == "suit") currentPlayer->getHand().sortBySuit();
            else if (mode == "value") currentPlayer->getHand().sortByValue();
            else cout << "Unknown sort mode.\n";
        }
        else {
            try {
                int index = stoi(firstWord);
                indices.push_back(index);
                int next;
                while (iss >> next) indices.push_back(next);

                if (indices.size() > 5) {
                    cout << "You can only discard up to 5 cards. Try again.\n";
                    indices.clear();
                    cout << "\nPress Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                size_t handSize = currentPlayer->getHand().getCards().size();
                bool allValid = all_of(indices.begin(), indices.end(), [&](int idx) {
                    return idx >= 0 && static_cast<size_t>(idx) < handSize;
                });

                if (!allValid) {
                    cout << "One or more indices are out of range. Try again.\n";
                    indices.clear();
                    cout << "\nPress Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
                }

                // Proceed with discarding
                vector<Card> discarded = currentPlayer->getHand().getCards();
                vector<Card> discardCopy;
                for (int idx : indices)
                    discardCopy.push_back(discarded[idx]);

                currentPlayer->discardCards(indices);
                vector<Card> newCards = deck.deal(indices.size());
                currentPlayer->addCardsToHand(newCards);
                discardRoundsLeft--;

                drawResultInterface(
                    *currentPlayer,
                    discardCopy,
                    currentPlayer->getHand().getCards(),
                    indices,
                    playRoundsLeft,
                    discardRoundsLeft,
                    "Discarded",
                    0, 1, 1,
                    {}
                );
                return;
            } catch (...) {
                cout << "Invalid input. Please try again.\n";
                cout << "\nPress Enter to continue...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                indices.clear();
            }
        }
    }
}

void GameManager::awardStage() {
    int finalScore = currentPlayer->getScore();
    int bestScore = Leaderboard::getSavedScore(currentPlayer->getUsername());

    drawAwardScreen(*currentPlayer, finalScore, bestScore);

    currentPlayer->addMoney(finalScore / 10);
    Leaderboard::updatePlayerScore(currentPlayer->getUsername(), finalScore);

    cout << "\nPress Enter to return to main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
