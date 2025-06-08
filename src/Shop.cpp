#include "Shop.h"
#include "Player.h"
#include "Item.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <map>
#include <limits>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>

using namespace std;

// Each tuple = (name, description, price)
const std::vector<std::tuple<std::string, std::string, int>> shopItems = {
    {"Score Double Ticket", "Doubles the score of the next played hand", 10},
    {"Spade Ticket", "Changes 3 random cards in hand to Spades", 5},
    {"Heart Ticket", "Changes 3 random cards in hand to Hearts", 5},
    {"Diamond Ticket", "Changes 3 random cards in hand to Diamonds", 5},
    {"Club Ticket", "Changes 3 random cards in hand to Clubs", 5},
    {"Copy Ticket", "Copies a randomly selected card in hand", 8}
};

void Shop::enterShopAnimation() {
    const string message = "Entering shopping mall...";

    for (size_t i = 1; i <= message.size(); ++i) {
        cout << "\033[2J\033[H";
        cout << "\033[1m" << message.substr(0, i) << "\033[0m" << flush;
        this_thread::sleep_for(chrono::milliseconds(80));
    }

    for (int i = 0; i < 2; ++i) {
        this_thread::sleep_for(chrono::milliseconds(200));
        cout << "\033[2J\033[H";
        this_thread::sleep_for(chrono::milliseconds(200));
        cout << "\033[1m" << message << "\033[0m" << flush;
    }

    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "\n\n";
}

void Shop::showItems() {
    cout << "\033[2J\033[H";

    const string title = "🛍️  SHOPPING MALL  🛍️";
    const int boxWidth = 60;
    const int screenWidth = 70;
    int padding = (screenWidth - title.length()) / 2;
    cout << string(padding, ' ') << "\033[1;36m" << title << "\033[0m\n\n";

    string horizontal = "─";
    string boxTop = "╭";
    string boxBot = "╰";
    for (int i = 0; i < boxWidth - 2; ++i) {
        boxTop += horizontal;
        boxBot += horizontal;
    }
    boxTop += "╮";
    boxBot += "╯";

    for (size_t i = 0; i < shopItems.size(); ++i) {
        const auto& [name, desc, price] = shopItems[i];
        string priceTag = "(\033[1;32m$" + to_string(price) + "\033[0m)";
        string nameLine = "[ " + to_string(i) + " ] \033[1m" + name + "\033[0m " + priceTag;

        cout << boxTop << "\n";
        cout << "│ " << left << setw(boxWidth - 3) << nameLine << "                   │" << endl;

        const int maxLineLen = boxWidth - 4;
        for (size_t j = 0; j < desc.length(); j += maxLineLen) {
            string line = desc.substr(j, maxLineLen);
            cout << "│ " << left << setw(boxWidth - 3) << line << "│" << endl;
        }

        cout << boxBot << "\n\n";
    }

    cout << "[x] Exit Shop\n";
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

        if (index < 0 || index >= static_cast<int>(shopItems.size()) || quantity <= 0) {
            std::cout << "Invalid item index or quantity.\n";
            continue;
        }

        cart[index] += quantity;

        std::cout << "\n✅ Added to cart: " << std::get<0>(shopItems[index]) << " x" << quantity << "\n";
        std::cout << "🛒 Current Cart:\n";

        int currentTotal = 0;
        for (const auto& [idx, qty] : cart) {
            int itemCost = std::get<2>(shopItems[idx]) * qty;
            std::cout << "- " << std::get<0>(shopItems[idx]) << " x" << qty
                      << " = $" << itemCost << "\n";
            currentTotal += itemCost;
        }
        std::cout << "Total so far: $" << currentTotal << "\n\n";
        std::cout << "\nPress Enter to continue...";
        std::cin.ignore();
        std::cin.get();
    }

    if (cart.empty()) {
        std::cout << "No items in cart. Exiting shop.\n";
        return;
    }

    int totalCost = 0;
    std::cout << "\nCheckout Summary:\n";
    for (const auto& [index, qty] : cart) {
        const auto& [name, desc, price] = shopItems[index];
        int cost = price * qty;
        std::cout << "- " << name << " x" << qty << " = $" << cost << "\n";
        totalCost += cost;
    }
    std::cout << "Total = $" << totalCost << "\n";

    if (totalCost > player.getMoney()) {
        std::cout << "Not enough money. Transaction canceled.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cout << "Confirm purchase? (y/n): ";
    std::string confirm;
    std::cin >> confirm;

    if (confirm == "y" || confirm == "Y") {
        for (const auto& [index, qty] : cart) {
            const std::string& itemName = std::get<0>(shopItems[index]);
            for (int i = 0; i < qty; ++i) {
                auto item = Item::fromName(itemName);
                if (item) player.addToInventory(std::move(item));
            }
        }
        player.addMoney(-totalCost);
        std::cout << "Purchase successful!\n";
    } else {
        std::cout << "Transaction canceled.\n";
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
