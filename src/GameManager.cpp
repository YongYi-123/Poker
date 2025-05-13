#include "GameManager.h"
#include <iostream>
#include <limits>

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
    currentPlayer->load();  // Load from JSON if available
    std::cout << "Welcome, " << currentPlayer->getName() << "!\n";
}

void GameManager::logout() {
    if (currentPlayer) {
        currentPlayer->save();  // Save to JSON
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
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                Leaderboard::show();  // static function to display leaderboard
                break;
            case 3:
                Shop::open(*currentPlayer);  // Pass player to shop
                break;
            case 4:
                logout();
                break;
            default:
                std::cout << "Invalid option.\n";
        }
    } while (currentPlayer);
}

void GameManager::playGame() {
    deck.reset();
    deck.shuffle();
    currentPlayer->drawHand(deck.deal(8));
    playRoundsLeft = 4;
    discardRoundsLeft = 3;

    while (playRoundsLeft > 0 && !currentPlayer->isHandEmpty()) {
        playRound();
        if (discardRoundsLeft > 0) {
            discardRound();
        }
    }

    awardStage();
}

void GameManager::playRound() {
    std::cout << "\n--- Play Round (" << playRoundsLeft << " left) ---\n";
    currentPlayer->showHand();

    std::vector<int> indices = currentPlayer->chooseCardsToPlay();  // User selects indices
    std::vector<Card> selected = currentPlayer->playCards(indices);

    int score = Scorer::calculateScore(selected);
    std::cout << "Scored " << score << " points!\n";
    currentPlayer->addScore(score);

    playRoundsLeft--;
}

void GameManager::discardRound() {
    std::cout << "\n--- Discard Round (" << discardRoundsLeft << " left) ---\n";
    currentPlayer->showHand();

    std::vector<int> indices = currentPlayer->chooseCardsToDiscard();  // User selects indices
    currentPlayer->discardCards(indices);

    std::vector<Card> newCards = deck.deal(indices.size());
    currentPlayer->addCardsToHand(newCards);

    discardRoundsLeft--;
}

void GameManager::awardStage() {
    std::cout << "\n=== Final Score: " << currentPlayer->getScore() << " ===\n";
    currentPlayer->showStats();
    currentPlayer->addMoney(currentPlayer->getScore() / 10);  // Bonus money
    Leaderboard::update(*currentPlayer);
}
