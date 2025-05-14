#include "GameManager.h"
#include "Leaderboard.h"
#include "Scorer.h"
#include "Shop.h"
#include "Display.h"
#include <iostream>
#include <limits>
#include <sstream>


GameManager::GameManager() {
    currentPlayer = nullptr;
    playRoundsLeft = 4;
    discardRoundsLeft = 3;
}

void GameManager::login() {
    std::string name;
    std::cout << "Enter player name to login: ";
    std::getline(std::cin, name);
    currentPlayer = new Player(name);
    currentPlayer->load();
    std::cout << "Welcome, " << currentPlayer->getUsername() << "!\n";
}

void GameManager::logout() {
    if (currentPlayer) {
        currentPlayer->save();
        delete currentPlayer;
        currentPlayer = nullptr;
        std::cout << "You have been logged out.\n";
    }
}

void GameManager::mainMenu() {
    int choice;
    do {
        std::cout << "\n=== Main Menu ===\n";
        std::cout << "1. Play Game\n";
        std::cout << "2. View Leaderboard\n";
        std::cout << "3. Shop\n";
        std::cout << "4. Logout\n";
        std::cout << "5. View Inventory\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                Leaderboard::displayTopPlayers(10, true);
                break;
            case 3:
                Shop::purchase(*currentPlayer);
                break;
            case 4:
                logout();
                break;
            case 5:
                std::cout << "\n=== Inventory ===\n";
                std::cout << "Balance: $" << currentPlayer->getMoney() << "\n";
                currentPlayer->displayInventory();
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (currentPlayer);
}

void GameManager::playGame() {
    currentPlayer->setScore(0);
    deck.reset();
    deck.shuffle();
    currentPlayer->drawHand(deck.deal(8));
    playRoundsLeft = 4;
    discardRoundsLeft = 3;

    while (playRoundsLeft > 0 && !currentPlayer->isHandEmpty()) {
        playRound();

        if (currentPlayer->isHandEmpty()) {
            std::cout << "No cards remaining. Ending game early.\n";
            break;
        }

        if (discardRoundsLeft > 0) {
            std::cout << "\nPress Enter to continue to discard round...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            discardRound();

            if (currentPlayer->isHandEmpty()) {
                std::cout << "No cards remaining after discard. Ending game early.\n";
                break;
            }
        }
    }

    std::cout << "\nPress Enter to continue to award stage...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    awardStage();
}


void GameManager::playRound() {
    std::vector<Card> selected;
    std::vector<int> indices;

    while (true) {
        drawHandOnlyInterface(
            *currentPlayer,
            currentPlayer->getHand().getCards(),
            playRoundsLeft,
            discardRoundsLeft
        );

        std::cout << "\n--- Play Round (" << playRoundsLeft << " left) ---\n";
        std::cout << "\nYou can:\n";
        std::cout << "- Type card indices to play (e.g. `0 1 3`)\n";
        std::cout << "- Type `sort suit` or `sort value`\n";
        std::cout << "- Type `show` to re-display your hand\n";
        std::cout << "- Type `cancel` to skip this round\n";
        std::cout << "Enter command or card indices: ";

        std::string inputLine;
        std::getline(std::cin, inputLine);
        std::istringstream iss(inputLine);

        std::string firstWord;
        iss >> firstWord;

        if (firstWord == "sort") {
            std::string mode;
            iss >> mode;
            if (mode == "suit") currentPlayer->getHand().sortBySuit();
            else if (mode == "value") currentPlayer->getHand().sortByValue();
            else std::cout << "Unknown sort mode.\n";
        } else if (firstWord == "show") {
            // Already shown at top of loop
        } else if (firstWord == "cancel") {
            std::cout << "Cancelled round. No cards played.\n";
            return;
        } else {
            try {
                int index = std::stoi(firstWord);
                indices.push_back(index);
                int next;
                while (iss >> next) indices.push_back(next);

                selected = currentPlayer->playCards(indices);

                if (selected.empty()) {
                    std::cout << "No valid cards selected.\n";
                    indices.clear();
                    continue;
                }
                break;
            } catch (...) {
                std::cout << "Invalid input. Try again.\n";
                indices.clear();
            }
        }
    }

    // Evaluate hand
    auto [handType, score] = Scorer::evaluate(selected);
    int multiplier = currentPlayer->getNextScoreMultiplier();
    if (multiplier > 1) {
        score *= multiplier;
        currentPlayer->setNextScoreMultiplier(1);
    }

    currentPlayer->addScore(score);
    currentPlayer->updateStats(handType);
    playRoundsLeft--;

    drawResultInterface(
        *currentPlayer,
        selected,
        currentPlayer->getHand().getCards(),
        indices,
        playRoundsLeft,
        discardRoundsLeft,
        handType,
        score
    );
}



void GameManager::discardRound() {
    std::vector<int> indices;

    while (true) {
        drawHandOnlyInterface(
            *currentPlayer,
            currentPlayer->getHand().getCards(),
            playRoundsLeft,
            discardRoundsLeft
        );

        std::cout << "\n--- Discard Round (" << discardRoundsLeft << " left) ---\n";
        std::cout << "\nYou can:\n";
        std::cout << "- Type card indices to discard (e.g. `0 1 3`)\n";
        std::cout << "- Type `sort suit` or `sort value`\n";
        std::cout << "- Type `show` to re-display your hand\n";
        std::cout << "- Type `cancel` to skip this discard round\n";
        std::cout << "Enter command or card indices: ";

        std::string inputLine;
        std::getline(std::cin, inputLine);
        std::istringstream iss(inputLine);

        std::string firstWord;
        iss >> firstWord;

        if (firstWord == "sort") {
            std::string mode;
            iss >> mode;
            if (mode == "suit") currentPlayer->getHand().sortBySuit();
            else if (mode == "value") currentPlayer->getHand().sortByValue();
            else std::cout << "Unknown sort mode.\n";
        } else if (firstWord == "show") {
            // Already shown
        } else if (firstWord == "cancel") {
            std::cout << "Discard round skipped.\n";
            return;
        } else {
            try {
                int index = std::stoi(firstWord);
                indices.push_back(index);
                int next;
                while (iss >> next) indices.push_back(next);

                std::vector<Card> discarded = currentPlayer->getHand().getCards();  // backup
                std::vector<Card> discardCopy;
                for (int idx : indices)
                    if (idx >= 0 && static_cast<size_t>(idx) < discarded.size())
                        discardCopy.push_back(discarded[idx]);

                currentPlayer->discardCards(indices);
                std::vector<Card> newCards = deck.deal(indices.size());
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
                    0
                );

                return;
            } catch (...) {
                std::cout << "Invalid input. Please try again.\n";
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

    std::cout << "\nPress Enter to return to main menu...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


