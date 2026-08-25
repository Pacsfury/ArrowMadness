#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

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
