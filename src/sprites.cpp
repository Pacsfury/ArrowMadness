#include <SFML/Graphics.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "../include/cards.hpp"
#include "../include/sprites.hpp"

inline std::vector<std::unique_ptr<sf::Texture>> g_Textures;
inline std::vector<std::string> g_TexturePaths;

int loadTexture(const std::string& path) {
    for (size_t i = 0; i < g_TexturePaths.size(); ++i) {
        if (g_TexturePaths[i] == path)
            return static_cast<int>(i);
    }

    auto tex = std::make_unique<sf::Texture>();
    if (tex->loadFromFile(path)) {
        g_Textures.push_back(std::move(tex));
        g_TexturePaths.push_back(path);
        return static_cast<int>(g_Textures.size() - 1);
    }
    return -1;
}

sf::Sprite newSprite(const std::string texturePath, bool setorigin) {
    int textureId = loadTexture(texturePath);

    if (textureId != -1) {
        auto s = sf::Sprite(*g_Textures[textureId]);
        if (setorigin)
            s.setOrigin({960.f, 540.f});
        return s;
    }
    return sf::Sprite(*g_Textures[textureId]);
}

std::vector<sf::Sprite> getDeckSprites() {
    float card_x = 100.f;
    std::filesystem::path dir_path = "img/cards";

    std::vector<sf::Sprite> sprite_deck;
    if (std::filesystem::exists(dir_path) && std::filesystem::is_directory(dir_path)) {
        std::vector<std::filesystem::path> entries;
        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            entries.push_back(entry.path());
        }
        std::sort(entries.begin(), entries.end());
        for (const auto& entry : entries) {
            auto tempsprite = newSprite(entry.string(), false);

            std::string name = entry.stem().string().substr(1);

            tempsprite.setScale({0.2, 0.2});
            tempsprite.setPosition({card_x, 800.f});

            if (hasCard(name)) {
                sprite_deck.push_back(tempsprite);
                card_x += 200.f;
            }
        }
    }
    return sprite_deck;
}