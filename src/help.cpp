#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../include/cards.hpp"
#include "../include/help.hpp"

bool checkHit(int targetDir, std::vector<int>& ballsDir, std::vector<float>& ballsY, sf::Text& points, int& lives) {
    bool hit = false;

    for (size_t i = 0; i < ballsDir.size(); ++i) {
        if (ballsDir[i] == targetDir && ballsY[i] > 390.f && ballsY[i] < 690.f) {
            ballsDir[i] = -1;
            int currentPoints = std::stoi(points.getString().toAnsiString());
            points.setString(std::to_string(currentPoints + (std::rand() % 10 == 0 && hasCard("SPECIAL.extrapoints") ? 3 : 1)));
            hit = true;
            break;
        }
    }

    return !hit && --lives < 1;
}

float getDir(int currDir) {
    switch (currDir) {
        case 2:  return 180.f;
        case 3:  return 90.f;
        case 4:  return -90.f;
        default: return 0.f;
    }
}
