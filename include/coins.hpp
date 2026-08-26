#pragma once
#include <SFML/Graphics.hpp>
#include <string>

#include "../include/datakeeper.hpp"
#include "../include/sprites.hpp"

sf::Sprite grantCoins(int coins, int& count, Table& table);
int readCoins(Table& table);