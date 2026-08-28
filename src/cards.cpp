#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

#include "../include/cards.hpp"

static const inline std::unordered_map<std::string, std::string> CARDS = 
{
    "COMMON.livesplusone", "lives++"
};


std::vector<Card> deck;

void addCard(std::string name) {
    Card card;
    card.name = name;
    auto it = CARDS.find(name);
    if (it == CARDS.end()) return;
    card.power = it->second;
    if (deck.size() < 4) {
        deck.push_back(card);
    } else {
        deck[2] = card;
    }
}

bool hasCard(std::string name) {
    auto it = std::find_if(deck.begin(), deck.end(), [&name](const Card& card) {
        return card.name == name;
    });

    if (it != deck.end()) {
        return true;
    } else {
        return false;
    }
}

std::string getPower(std::string name) {
    auto it = std::find_if(deck.begin(), deck.end(), [&name](const Card& card) {
        return card.name == name;
    });

    if (it != deck.end()) {
        return it->power;
    } else {
        return it->power;
    }
}