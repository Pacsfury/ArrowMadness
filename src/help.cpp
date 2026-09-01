#include <SFML/Graphics.hpp>
#include <cstdlib>
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
            if (std::rand() % 10 == 0 && hasCard("SPECIAL.extrapoints")) {
                points.setString(std::to_string(currentPoints + 3));
            } else {
                points.setString(std::to_string(currentPoints + 1));
            }
            hit = true;
            break;
        }
    }

    if (!hit) {
        lives -= 1;
        if (lives < 1) {
            return true;
        }
    }
    return false;
}

float getDir(int currDir) {
    if (currDir == 2) {
        return 180.f;
    } else if (currDir == 3) {
        return 90.f;
    } else if (currDir == 4) {
        return -90.f;
    }
    return 0.f;
}
