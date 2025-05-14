#include "GameManager.h"
#include "Leaderboard.h"
#include "Scorer.h"
#include "Shop.h"
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
                Leaderboard::displayTopPlayers();
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
    currentPlayer->setScore(0);  // <--- add this
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
            discardRound();

            if (currentPlayer->isHandEmpty()) {
                std::cout << "No cards remaining after discard. Ending game early.\n";
                break;
            }
        }
    }

    awardStage();
}

void GameManager::playRound() {
    std::cout << "\n--- Play Round (" << playRoundsLeft << " left) ---\n";

    if (currentPlayer->isHandEmpty()) {
        std::cout << "You have no cards left to play.\n";
        return;
    }

    // Let player use an item
    if (!currentPlayer->getInventory().empty()) {
        std::cout << "Use an item before playing? (y/n): ";
        char useItem;
        std::cin >> useItem;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (useItem == 'y' || useItem == 'Y') {
            currentPlayer->displayInventory();
            std::cout << "Enter exact item name to use: ";
            std::string item;
            std::getline(std::cin, item);

            if (item == "Score x2 Ticket") {
                currentPlayer->setNextScoreMultiplier(2);
                currentPlayer->useItem(item);
                std::cout << "Score multiplier will be applied!\n";
            } else if (item == "Copy Ticket") {
                if (!currentPlayer->isHandEmpty()) {
                    currentPlayer->copyRandomCardInHand();
                    currentPlayer->useItem(item);
                    std::cout << "A random card was copied!\n";
                }
            } else if (item == "Spade Ticket" || item == "Heart Ticket" ||
                       item == "Diamond Ticket" || item == "Club Ticket") {
                Suit targetSuit;
                if (item.find("Spade") != std::string::npos) targetSuit = Spades;
                else if (item.find("Heart") != std::string::npos) targetSuit = Hearts;
                else if (item.find("Diamond") != std::string::npos) targetSuit = Diamonds;
                else targetSuit = Clubs;

                currentPlayer->changeCardSuits(targetSuit, 3);
                currentPlayer->useItem(item);
                std::cout << "3 cards were changed to " << item << ".\n";
            } else {
                std::cout << "Invalid item.\n";
            }
        }
    }

    // Sorting menu
    while (true) {
        std::cout << "\nSort your hand?\n";
        std::cout << "1. Sort by Value\n";
        std::cout << "2. Sort by Suit\n";
        std::cout << "3. No sorting\n";
        std::cout << "4. Proceed to card selection\n";
        std::cout << "Enter your choice: ";
        int sortChoice;
        std::cin >> sortChoice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (sortChoice == 1)
            currentPlayer->getHand().sortByValue();
        else if (sortChoice == 2)
            currentPlayer->getHand().sortBySuit();
        else if (sortChoice == 4)
            break;

        currentPlayer->showHand();
    }


    std::vector<int> indices = currentPlayer->chooseCardsToPlay();
    std::vector<Card> selected = currentPlayer->playCards(indices);

    auto [handType, score] = Scorer::evaluate(selected);
    int multiplier = currentPlayer->getNextScoreMultiplier();
    if (multiplier > 1) {
        std::cout << "Score x" << multiplier << " applied!\n";
        score *= multiplier;
        currentPlayer->setNextScoreMultiplier(1); // reset
    }

    std::cout << "Hand type: " << handType << "\n";
    std::cout << "Scored " << score << " points!\n";

    currentPlayer->addScore(score);
    currentPlayer->updateStats(handType);
    playRoundsLeft--;
}

void GameManager::discardRound() {
    std::cout << "\n--- Discard Round (" << discardRoundsLeft << " left) ---\n";

    int sortChoice;
    std::cout << "Sort your hand?\n";
    std::cout << "1. Sort by Value\n";
    std::cout << "2. Sort by Suit\n";
    std::cout << "3. No sorting\n";
    std::cout << "Enter your choice: ";
    std::cin >> sortChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (sortChoice == 1)
        currentPlayer->getHand().sortByValue();
    else if (sortChoice == 2)
        currentPlayer->getHand().sortBySuit();

    currentPlayer->showHand();

    std::vector<int> indices = currentPlayer->chooseCardsToDiscard();
    currentPlayer->discardCards(indices);

    std::vector<Card> newCards = deck.deal(indices.size());
    currentPlayer->addCardsToHand(newCards);

    discardRoundsLeft--;
}

void GameManager::awardStage() {
    int finalScore = currentPlayer->getScore();
    int bestScore = Leaderboard::getSavedScore(currentPlayer->getUsername());

    std::cout << "\n=== Final Score: " << finalScore << " ===\n";
    std::cout << "Personal Best: " << std::max(finalScore, bestScore) << "\n";

    currentPlayer->showStats();
    currentPlayer->addMoney(currentPlayer->getScore() / 10);
    Leaderboard::updatePlayerScore(currentPlayer->getUsername(), currentPlayer->getScore());
}
