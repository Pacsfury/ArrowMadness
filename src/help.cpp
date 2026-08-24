#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../include/help.hpp"

void checkHit(int targetDir, std::vector<int>& ballsDir, std::vector<float>& ballsY, sf::Text& points) {
    bool hit = false;

    for (size_t i = 0; i < ballsDir.size(); ++i) {
        if (ballsDir[i] == targetDir && ballsY[i] > 390.f && ballsY[i] < 690.f) {
            ballsDir[i] = -1;
            int currentPoints = std::stoi(points.getString().toAnsiString());
            points.setString(std::to_string(currentPoints + 1));
            hit = true;
            break;
        }
    }

    if (!hit) {
        points.setString("0");
    }
}

float getDir(int currDir) {
    if (currDir == 1) {
        return 0.f;
    } else if (currDir == 2) {
        return 180.f;
    } else if (currDir == 3) {
        return 90.f;
    } else if (currDir == 4) {
        return -90.f;
    }
    return 0.f;
}
