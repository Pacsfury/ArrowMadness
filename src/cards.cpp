#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../include/cards.hpp"

static const inline std::vector<std::string> CARDS = {"COMMON.livesplusone", "SPECIAL.extrapoints"};

std::vector<std::string> deck;

void addCard(std::string name) {
    std::string card = name;

    if (deck.size() < 4) {
        deck.push_back(card);
    } else {
        deck[2] = card;
    }
}

bool hasCard(std::string name) {
    return std::find(deck.begin(), deck.end(), name) != deck.end();
}