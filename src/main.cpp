#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Arrow Madness");

    sf::Texture texture;
    texture.loadFromFile("img/background.png");
    sf::Sprite background(texture);

    sf::Texture arrowTexture;
    arrowTexture.loadFromFile("img/arrow.png");
    sf::Sprite up(arrowTexture);
    sf::Sprite down(arrowTexture);
    sf::Sprite right(arrowTexture);
    sf::Sprite left(arrowTexture);

    sf::Font font("fonts/Super Bouncer.ttf"); 
    sf::Text points(font);
    points.setPosition({960.f, 0.f});
    points.setCharacterSize(90);
    points.setString("0");

    sf::FloatRect bounds = up.getLocalBounds();
    up.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    down.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    right.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    left.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

    up.setPosition({960.f, 440.f});
    down.setPosition({960.f, 640.f});
    right.setPosition({860.f, 540.f});
    left.setPosition({1060.f, 540.f});

    sf::Angle Angle = sf::degrees(180.f);
    down.setRotation(Angle);
    Angle = sf::degrees(-90.f);
    right.setRotation(Angle);
    Angle = sf::degrees(90.f);
    left.setRotation(Angle);
    
    sf::Color clicked(255, 255, 255, 128);
    sf::Color released(255, 255, 255, 255);

    std::vector<int>   ballsDir = {1, 3};
    std::vector<float> ballsX   = {130.f, 180.f};
    std::vector<float> ballsY   = {400.f, 130.f};

    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<float> timeDist(2.0f, 5.0f);     
    std::uniform_int_distribution<int> dirDist(1, 4);              
    std::uniform_real_distribution<float> xDist(100.f, 1800.f);     

    sf::Clock spawnClock;
    float nextSpawnTime = timeDist(gen); 

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } 
            else if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseClick->button == sf::Mouse::Button::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(mouseClick->position);

                    if (up.getGlobalBounds().contains(mousePos)) {
                        up.setColor(clicked);
                        auto it = std::find(ballsDir.begin(), ballsDir.end(), 1);

                        if (it != ballsDir.end()) {
                            *it = -1;
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    } 
                    if (down.getGlobalBounds().contains(mousePos)) {
                        down.setColor(clicked);
                        auto it = std::find(ballsDir.begin(), ballsDir.end(), 2);

                        if (it != ballsDir.end()) {
                            *it = -1;
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    }
                    if (left.getGlobalBounds().contains(mousePos)) {
                        left.setColor(clicked);
                        auto it = std::find(ballsDir.begin(), ballsDir.end(), 3);

                        if (it != ballsDir.end()) {
                            *it = -1;
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    }
                    if (right.getGlobalBounds().contains(mousePos)) {
                        right.setColor(clicked);
                        auto it = std::find(ballsDir.begin(), ballsDir.end(), 4);

                        if (it != ballsDir.end()) {
                            *it = -1;
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    }
                }
            }

            else if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseRelease->button == sf::Mouse::Button::Left) {
                    up.setColor(released);
                    down.setColor(released);
                    left.setColor(released);
                    right.setColor(released);
                }
            }
        }

        if (spawnClock.getElapsedTime().asSeconds() >= nextSpawnTime) {
            ballsDir.push_back(dirDist(gen)); 
            ballsX.push_back(xDist(gen)); 
            ballsY.push_back(0.f);           

            spawnClock.restart();            
            nextSpawnTime = timeDist(gen);
        }

        window.clear();
        window.draw(background);
        window.draw(up);
        window.draw(down);
        window.draw(right);
        window.draw(left);
        window.draw(points);
        
        for (size_t i = 0; i < ballsDir.size(); i++) {
            if (ballsDir[i] == -1) continue;

            sf::Texture ballTexture;
            ballTexture.loadFromFile("img/ball.png");
            sf::Sprite ballSprite(ballTexture);

            float dir = 0.f;
            int currDir = ballsDir[i];

            if (currDir == 1) {
                dir = 0.f;
            } else if (currDir == 2) {
                dir = 180.f;
            } else if (currDir == 3) {
                dir = 90.f;
            } else if (currDir == 4) {
                dir = -90.f;
            }

            sf::Angle ballAngle = sf::degrees(dir);
            ballSprite.setRotation(ballAngle);
            ballSprite.setPosition({ballsX[i], ballsY[i]});
            ballsY[i] += 0.1f;
            window.draw(ballSprite);
        }
        window.display();
    }

    return 0;
}
