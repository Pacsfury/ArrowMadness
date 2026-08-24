#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

void checkHit(int targetDir, std::vector<int>& ballsDir, std::vector<float>& ballsY, sf::Text& points);

float getDir(int currDir);