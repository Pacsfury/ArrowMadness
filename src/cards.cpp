#include <vector>
#include <iostream>
#include <string>
#include <algorithm>

#include "../include/cards.hpp"

static const inline std::vector<std::string> CARDS = 
{
    "COMMON.livesplusone",
    "SPECIAL.extrapoints"
};


std::vector<std::string> deck;

void addCard(std::string name) {
    std::string card;
    card = name;

    if (deck.size() < 4) {
        deck.push_back(card);
    } else {
        deck[2] = card;
    }
}

bool hasCard(std::string name) {
    auto it = std::find_if(deck.begin(), deck.end(), [&name](const std::string& card) {
        return card == name;
    });

    if (it != deck.end()) {
        return true;
    } else {
        return false;
    }
}
