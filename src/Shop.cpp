#include "Shop.h"
#include <iostream>
#include <map>
#include <limits>
#include <algorithm>

using namespace std;

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
    std::cout << "\033[H\033[J";
    std::cout << "==== Shop Menu ====\n";
    for (size_t i = 0; i < shopItems.size(); ++i) {
        std::cout << "[" << i << "] "
                  << shopItems[i].name << " ($" << shopItems[i].price << ") - "
                  << shopItems[i].description << "\n";
    }
    std::cout << "[x] Exit Shop\n";
}

void Shop::purchase(Player& player) {
    std::map<int, int> cart;

    while (true) {
        showItems();
        std::cout << "Current Balance: $" << player.getMoney() << "\n";
        std::cout << "Enter purchases like: index quantity (e.g. 0 2), or 'x' to checkout.\n> ";

        std::string input;
        std::cin >> input;

        if (input == "x" || input == "X") break;

        int index, quantity;
        try {
            index = std::stoi(input);
            std::cin >> quantity;
        } catch (...) {
            std::cout << "Invalid input. Please enter item index followed by quantity.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (index < 0 || index >= (int)shopItems.size() || quantity <= 0) {
            std::cout << "Invalid item index or quantity.\n";
            continue;
        }

        cart[index] += quantity;

        std::cout << "\n✅ Added to cart: " << shopItems[index].name << " x" << quantity << "\n";
        std::cout << "🛒 Current Cart:\n";

        int currentTotal = 0;
        for (const auto& [idx, qty] : cart) {
            int itemCost = shopItems[idx].price * qty;
            std::cout << "- " << shopItems[idx].name << " x" << qty
                    << " = $" << itemCost << "\n";
            currentTotal += itemCost;
        }
        std::cout << "Total so far: $" << currentTotal << "\n\n";
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore(); // consume leftover '\n' from previous input
        std::cin.get();    // actually wait for Enter
    }

    if (cart.empty()) {
        std::cout << "No items in cart. Exiting shop.\n";
        return;
    }

    // Checkout summary
    int totalCost = 0;
    std::cout << "\nCheckout Summary:\n";
    for (const auto& [index, qty] : cart) {
        const Item& item = shopItems[index];
        int cost = item.price * qty;
        std::cout << "- " << item.name << " x" << qty << " = $" << cost << "\n";
        totalCost += cost;
    }
    std::cout << "Total = $" << totalCost << "\n";

    if (totalCost > player.getMoney()) {
        std::cout << "Not enough money. Transaction canceled.\n";
        return;
    }

    std::cout << "Confirm purchase? (y/n): ";
    std::string confirm;
    std::cin >> confirm;

    if (confirm == "y" || confirm == "Y") {
        for (const auto& [index, qty] : cart) {
            for (int i = 0; i < qty; ++i)
                player.addToInventory(shopItems[index].name);
        }
        player.addMoney(-totalCost);
        std::cout << "Purchase successful!\n";
    } else {
        std::cout << "Transaction canceled.\n";
    }
}
