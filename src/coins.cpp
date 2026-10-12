#include <SFML/Graphics.hpp>
#include <string>

#include "../include/datakeeper.hpp"
#include "../include/sprites.hpp"

sf::Sprite grantCoins(int coins, int& count, Table& table) {
    cleanGrantCoins(coins, count, table);
    return newSprite("img/coinreward.png", false);
}

void cleanGrantCoins(int coins, int& count, Table& table) {
    count += coins;
    table.save("coins", std::to_string(count));
}

int readCoins(Table& table) { return std::stoi(table.get("coins")); }