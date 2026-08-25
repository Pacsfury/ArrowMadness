#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

int loadTexture(const std::string& path);

sf::Sprite newSprite(const std::string texturePath, bool setorigin = true);
