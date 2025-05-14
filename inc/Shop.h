#ifndef SHOP_H
#define SHOP_H

#include "Player.h"

class Shop {
public:
    // Show available items in the shop
    static void showItems();

    // Let the player purchase an item
    static void purchase(Player& player);
};

#endif
