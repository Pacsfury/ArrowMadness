#include <SFML/Graphics.hpp>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "../include/datakeeper.hpp"
#include "../include/help.hpp"
#include "../include/sprites.hpp"

enum class screens : uint16_t { GAME, OVER, MENU, SHOP };

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Arrow Madness");
    window.setVerticalSyncEnabled(true);

    Table userData(1);
    userData.save("totalPoints", userData.get("totalPoints") == "" ? "0" : userData.get("totalPoints"));
    userData.save("coins", userData.get("coins") == "" ? "0" : userData.get("coins"));

    sf::Image icon;
    icon.loadFromFile("img/icon.png");
    window.setIcon(icon);

    sf::RectangleShape line(sf::Vector2f(1920.f, 200.f));
    line.setPosition({0.f, 440.f});
    line.setFillColor(sf::Color(255, 100, 100, 128));

    auto background = newSprite("img/background.png", false);
    auto up = newSprite("img/arrow.png");
    auto down = newSprite("img/arrow.png");
    auto right = newSprite("img/arrow.png");
    auto left = newSprite("img/arrow.png");
    auto heart = newSprite("img/heart.png", false);
    auto replay = newSprite("img/replay.png");
    auto menu = newSprite("img/menu.png");
    auto quit = newSprite("img/quit.png");
    auto broken = newSprite("img/brokenh.png", false);
    auto play = newSprite("img/play.png");
    auto shop = newSprite("img/shop.png");
    auto starterPackOffer = newSprite("img/starter.png", false);
    auto back = newSprite("img/back.png");
    auto ball = newSprite("img/ball.png", false);

    sf::FloatRect rebounds = replay.getLocalBounds();
    replay.setPosition({960.f, 360.f});
    replay.setScale({0.25, 0.25});
    menu.setPosition({960.f, 600.f});
    menu.setScale({0.25, 0.25});
    quit.setPosition({960.f, 840.f});
    quit.setScale({0.25, 0.25});
    play.setPosition({1670.f, 870.f});
    play.setScale({0.25, 0.25});
    shop.setPosition({1670.f, 620.f});
    shop.setScale({0.25, 0.25});
    starterPackOffer.setScale({0.5, 0.5});
    back.setPosition({1670.f, 870.f});
    back.setScale({0.25, 0.25});

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
    gameover.setOrigin({gobounds.position.x + gobounds.size.x / 2.f, gobounds.position.y + gobounds.size.y / 2.f});

    gameover.setPosition({960.f, 120.f});

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
    screens screen = screens::GAME;

    std::vector<int> ballsDir = {1, 3};
    std::vector<float> ballsX = {130.f, 180.f};
    std::vector<float> ballsY = {400.f, 130.f};

    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<float> timeDist(0.5f, 3.0f);
    std::uniform_int_distribution<int> dirDist(1, 4);
    std::uniform_real_distribution<float> xDist(100.f, 1800.f);

    sf::Clock spawnClock;
    float nextSpawnTime = timeDist(gen);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();

            } else if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseClick->position);

                switch (screen) {
                    case screens::GAME:
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
                                userData.load();
                                userData.save("totalPoints",
                                              std::to_string(std::stoi(userData.get("totalPoints")) +
                                                             std::stoi(points.getString().toAnsiString())));
                                userData.file();
                                points.setString("0");
                                screen = screens::OVER;
                            }
                        }
                        break;
                    case screens::OVER:
                        if (replay.getGlobalBounds().contains(mousePos)) {
                            points.setString("0");
                            screen = screens::GAME;
                            lives = 3;
                        } else if (quit.getGlobalBounds().contains(mousePos)) {
                            window.close();
                        } else if (menu.getGlobalBounds().contains(mousePos)) {
                            screen = screens::MENU;
                        }
                        break;
                    case screens::MENU:
                        if (play.getGlobalBounds().contains(mousePos)) {
                            screen = screens::GAME;
                            lives = 3;
                        } else if (shop.getGlobalBounds().contains(mousePos)) {
                            screen = screens::SHOP;
                        }
                        break;
                    case screens::SHOP:
                        if (back.getGlobalBounds().contains(mousePos)) {
                            screen = screens::MENU;
                        }
                        break;
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

        switch (screen) {
            case screens::GAME: {
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

                    ball.setRotation(sf::degrees(getDir(ballsDir[i])));
                    ball.setPosition({ballsX[i], ballsY[i]});
                    ballsY[i] += 10.f;

                    window.draw(ball);
                }

                window.draw(up);
                window.draw(down);
                window.draw(right);
                window.draw(left);
                window.draw(points);

                for (int i = 0; i < 3; i++) {
                    if (lives > i) {
                        heart.setPosition({10.f + (i * 120.f), 10.f});
                        window.draw(heart);
                        continue;
                    }
                    broken.setPosition({10.f + (i * 120.f), 10.f});
                    window.draw(broken);
                }
            } break;
            case screens::OVER: {
                window.clear();
                window.draw(background);
                window.draw(gameover);
                window.draw(replay);
                window.draw(menu);
                window.draw(quit);
            } break;
            case screens::MENU: {
                window.clear();
                window.draw(background);
                window.draw(play);
                window.draw(shop);
            } break;
            case screens::SHOP: {
                window.clear();
                window.draw(background);
                window.draw(starterPackOffer);
                window.draw(back);
            } break;
        }

        window.display();
    }

    return 0;
}
