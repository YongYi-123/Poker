#ifndef SHOP_H
#define SHOP_H

#include <string>
#include <vector>
#include "Player.h"

class Shop {
public:
    static void showItems();
    static void purchase(Player& player);
};

#endif
 