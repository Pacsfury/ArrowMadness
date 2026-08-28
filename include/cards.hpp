#include <vector>
#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

struct Card {
    std::string name;
    std::string power;
};

void addCard(std::string name);

bool hasCard(std::string name);

std::string getPower(std::string name);