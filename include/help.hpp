#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

bool checkHit(int targetDir, std::vector<int>& ballsDir, std::vector<float>& ballsY, sf::Text& points, int& lives);

float getDir(int currDir);