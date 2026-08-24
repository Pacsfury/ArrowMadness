#include <SFML/Graphics.hpp>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "../include/help.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Arrow Madness");
    window.setVerticalSyncEnabled(true);

    sf::Image icon;
    icon.loadFromFile("img/icon.png");
    window.setIcon(icon);

    sf::Texture texture;
    texture.loadFromFile("img/background.png");
    sf::Sprite background(texture);

    sf::RectangleShape line(sf::Vector2f(1920.f, 200.f));
    line.setPosition({0.f, 440.f});
    line.setFillColor(sf::Color(0, 255, 0, 50));
    sf::Color lineColor(255, 100, 100, 128);
    line.setFillColor(lineColor);

    sf::Texture arrowTexture;
    sf::Texture heartTexture;
    sf::Texture replayTexture;
    heartTexture.loadFromFile("img/heart.png");
    arrowTexture.loadFromFile("img/arrow.png");
    replayTexture.loadFromFile("img/replay.png");
    sf::Sprite up(arrowTexture);
    sf::Sprite down(arrowTexture);
    sf::Sprite right(arrowTexture);
    sf::Sprite left(arrowTexture);
    sf::Sprite heart(heartTexture);
    sf::Sprite replay(replayTexture);
    sf::FloatRect rebounds = replay.getLocalBounds();
    replay.setOrigin({rebounds.size.x / 2.f, rebounds.size.y / 2.f});
    replay.setPosition({960.f, 540.f});
    replay.setScale({0.3, 0.3});

    sf::FloatRect bounds = up.getLocalBounds();

    sf::Font font("fonts/Super Bouncer.ttf");
    sf::Text points(font);
    points.setPosition({960.f, 0.f});
    points.setCharacterSize(90);
    points.setString("0");

    sf::Text gameover(font);
    gameover.setString("Game Over!");
    gameover.setCharacterSize(160);

    sf::FloatRect gobounds = gameover.getLocalBounds();
    gameover.setOrigin({
        gobounds.position.x + gobounds.size.x / 2.f, 
        gobounds.position.y + gobounds.size.y / 2.f
    });

    gameover.setPosition({960.f, 300.f});

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

    int lives = 3;
    std::string screen = "GAME";

    std::vector<int> ballsDir = {1, 3};
    std::vector<float> ballsX = {130.f, 180.f};
    std::vector<float> ballsY = {400.f, 130.f};

    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<float> timeDist(0.5f, 3.0f);
    std::uniform_int_distribution<int> dirDist(1, 4);
    std::uniform_real_distribution<float> xDist(100.f, 1800.f);

    sf::Clock spawnClock;
    float nextSpawnTime = timeDist(gen);

    sf::Texture ballTexture;
    ballTexture.loadFromFile("img/ball.png");
    sf::Sprite ballSprite(ballTexture);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();

            } else if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseClick->position);
                if (screen == "GAME") {
                    if (mouseClick->button == sf::Mouse::Button::Left) {
                        bool res = false;

                        if (up.getGlobalBounds().contains(mousePos)) {
                            up.setColor(clicked);
                            res = checkHit(1, ballsDir, ballsY, points, lives);
                        } else if (down.getGlobalBounds().contains(mousePos)) {
                            down.setColor(clicked);
                            res = checkHit(2, ballsDir, ballsY, points, lives);
                        } else if (left.getGlobalBounds().contains(mousePos)) {
                            left.setColor(clicked);
                            res = checkHit(3, ballsDir, ballsY, points, lives);
                        } else if (right.getGlobalBounds().contains(mousePos)) {
                            right.setColor(clicked);
                            res = checkHit(4, ballsDir, ballsY, points, lives);
                        }

                        if (res) {
                            screen = "OVER";
                        }
                    }
                } else if (screen == "OVER") {
                    if (replay.getGlobalBounds().contains(mousePos)) {
                        screen = "GAME";
                        lives = 3;
                    }
                }
            } else if (const auto* mouseRelease = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseRelease->button == sf::Mouse::Button::Left) {
                    up.setColor(released);
                    down.setColor(released);
                    left.setColor(released);
                    right.setColor(released);
                }
            }
        }

        if (screen == "GAME") {
            if (spawnClock.getElapsedTime().asSeconds() >= nextSpawnTime) {
                ballsDir.push_back(dirDist(gen));
                ballsX.push_back(xDist(gen));
                ballsY.push_back(0.f);

                spawnClock.restart();
                nextSpawnTime = timeDist(gen);
            }

            window.clear();
            window.draw(background);
            window.draw(line);

            for (size_t i = 0; i < ballsDir.size(); i++) {
                if (ballsDir[i] == -1)
                    continue;

                ballSprite.setRotation(sf::degrees(getDir(ballsDir[i])));
                ballSprite.setPosition({ballsX[i], ballsY[i]});
                ballsY[i] += 10.f;

                window.draw(ballSprite);
            }

            window.draw(up);
            window.draw(down);
            window.draw(right);
            window.draw(left);
            window.draw(points);

            for (int i = 0; i < lives ; i++) {
                heart.setPosition({10.f + (i*120.f), 10.f});
                window.draw(heart);
            }
        } else if (screen == "OVER") {
            window.draw(background);
            window.draw(gameover);
            window.draw(replay);
        }

        window.display();
    }

    return 0;
}
