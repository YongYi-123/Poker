#include "Shop.h"
#include <iostream>
#include <map>

struct Item {
    std::string name;
    std::string description;
    int price;
};

// Define available shop items
const std::vector<Item> shopItems = {
    {"Score Double Ticket", "Doubles the score of the next played hand", 10},
    {"Spade Ticket", "Changes 3 random cards in hand to Spades", 5},
    {"Heart Ticket", "Changes 3 random cards in hand to Hearts", 5},
    {"Diamond Ticket", "Changes 3 random cards in hand to Diamonds", 5},
    {"Club Ticket", "Changes 3 random cards in hand to Clubs", 5},
    {"Copy Ticket", "Copies a randomly selected card in hand", 8}
};

void Shop::showItems() {
    std::cout << "==== Shop Menu ====\n";
    for (size_t i = 0; i < shopItems.size(); ++i) {
        std::cout << "[" << i << "] "
                  << shopItems[i].name << " ($" << shopItems[i].price << ") - "
                  << shopItems[i].description << "\n";
    }
    std::cout << "[x] Exit Shop\n";
}

void Shop::purchase(Player& player) {
    while (true) {
        showItems();
        std::cout << "Current Balance: $" << player.getMoney() << "\n";
        std::cout << "Enter item index to purchase or 'x' to exit: ";

        std::string input;
        std::cin >> input;

        if (input == "x" || input == "X") {
            std::cout << "Exiting shop...\n";
            break;
        }

        int choice;
        try {
            choice = std::stoi(input);
        } catch (...) {
            std::cout << "Invalid input.\n";
            continue;
        }

        if (choice < 0 || choice >= (int)shopItems.size()) {
            std::cout << "Invalid item index.\n";
            continue;
        }

        const Item& selected = shopItems[choice];
        if (player.getMoney() < selected.price) {
            std::cout << "Not enough money.\n";
        } else {
            player.addMoney(-selected.price);
            player.addToInventory(selected.name);
            std::cout << "Purchased: " << selected.name << "\n";
        }
    }
}
