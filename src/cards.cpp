#include <algorithm>
#include <array>
#include <iostream>
#include <string>

#include "../include/cards.hpp"

static const inline std::array<std::string, 3> CARDS = {"COMMON.livesplusone", "SPECIAL.extrapoints", "MYTHIC.coinballs"};

std::array<std::string, 3> deck;
static size_t card_count = 0; 

void cards_init() {
    deck.fill(""); 
}

void addCard(const std::string& card) {
    deck[std::min(card_count++, size_t(2))] = card;
}

bool hasCard(const std::string& name) { 
    return std::find(deck.begin(), deck.end(), name) != deck.end(); 
}
