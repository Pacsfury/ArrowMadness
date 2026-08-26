#include <SFML/Graphics.hpp>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>

#include "../include/coins.hpp"
#include "../include/datakeeper.hpp"
#include "../include/help.hpp"
#include "../include/sprites.hpp"

enum class screens : uint16_t { GAME, OVER, MENU, SHOP, BOX, COIN };

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Arrow Madness");
    window.setVerticalSyncEnabled(true);

    Table userData(1);
    userData.load();
    userData.save("totalPoints", userData.get("totalPoints") == "" ? "0" : userData.get("totalPoints"));
    userData.save("coins", userData.get("coins") == "" ? "0" : userData.get("coins"));
    userData.save("offers.starterpack.claimed", userData.get("offers.starterpack.claimed") == "1" ? "1" : "0");
    userData.save("username", userData.get("username") == "" ? "Guest" : userData.get("username"));
    userData.file();

    int coins = std::stoi(userData.get("coins"));

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
    auto boxscreen = newSprite("img/boxscreen.png", false);
    auto coin = newSprite("img/coin.png", false);
    auto free = newSprite("img/free.png", false);

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
    coin.setScale({0.1, 0.1});
    coin.setPosition({1680.f, -22.f});
    free.setScale({0.25, 0.25});
    free.setPosition({1240.f, 390.f});

    sf::FloatRect bounds = up.getLocalBounds();

    sf::Font font("fonts/Super Bouncer.ttf");
    sf::Text points(font);
    points.setPosition({960.f, 0.f});
    points.setCharacterSize(90);
    points.setString("0");

    sf::Text gameover(font);
    sf::Text coinCount(font);
    sf::Text usernameText(font);
    sf::Text totalPointsText(font);
    usernameText.setString(userData.get("username"));
    usernameText.setPosition({150.f, 10.f});
    usernameText.setCharacterSize(80);
    totalPointsText.setString(userData.get("totalPoints"));
    totalPointsText.setPosition({150.f, 100.f});
    totalPointsText.setCharacterSize(40);
    coinCount.setString(std::to_string(coins));
    coinCount.setPosition({1800.f, 10.f});
    gameover.setString("Game Over!");
    gameover.setCharacterSize(160);
    sf::Text gotCoins(font);
    gotCoins.setCharacterSize(160);
    gotCoins.setPosition({960.f, 650.f});
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
    int coins_last;
    std::vector<screens> screenBuf = {screens::MENU};

    std::vector<int> ballsDir = {1, 3};
    std::vector<float> ballsX = {130.f, 180.f};
    std::vector<float> ballsY = {400.f, 130.f};

    std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_real_distribution<float> timeDist(0.5f, 3.0f);
    std::uniform_int_distribution<int> dirDist(1, 4);
    std::uniform_real_distribution<float> xDist(100.f, 1800.f);
    std::uniform_int_distribution<int> randcoins(10, 60);

    sf::Sprite coin_sprite = newSprite("img/coinreward.png");

    sf::Clock spawnClock;
    float nextSpawnTime = timeDist(gen);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                userData.file();
                window.close();

            } else if (const auto* mouseClick = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(mouseClick->position);

                switch (screenBuf.back()) {
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
                                screenBuf.back() = screens::OVER;
                            }
                        }
                        break;
                    case screens::OVER:
                        if (replay.getGlobalBounds().contains(mousePos)) {
                            points.setString("0");
                            screenBuf.back() = screens::GAME;
                            lives = 3;
                        } else if (quit.getGlobalBounds().contains(mousePos)) {
                            window.close();
                        } else if (menu.getGlobalBounds().contains(mousePos)) {
                            screenBuf.back() = screens::MENU;
                        }
                        totalPointsText.setString(userData.get("totalPoints"));
                        break;
                    case screens::MENU:
                        if (play.getGlobalBounds().contains(mousePos)) {
                            screenBuf.back() = screens::GAME;
                            lives = 3;
                        } else if (shop.getGlobalBounds().contains(mousePos)) {
                            screenBuf.push_back(screens::SHOP);
                        }
                        break;
                    case screens::SHOP:
                        if (back.getGlobalBounds().contains(mousePos)) {
                            screenBuf.pop_back();
                        } else if (starterPackOffer.getGlobalBounds().contains(mousePos)) {
                            coin_sprite = grantCoins(50, coins, userData);
                            coins_last = 50;
                            screenBuf.push_back(screens::BOX);
                            screenBuf.push_back(screens::COIN);
                            userData.save("offers.starterpack.claimed", "1");
                            userData.file();
                        }
                        break;
                    case screens::BOX:
                        if (boxscreen.getGlobalBounds().contains(mousePos)) {
                            screenBuf.pop_back();
                            int given = randcoins(gen);
                            coin_sprite = grantCoins(given, coins, userData);
                            coins_last = given;
                            screenBuf.push_back(screens::COIN);
                        }
                        break;
                    case screens::COIN:
                        if (coin_sprite.getGlobalBounds().contains(mousePos)) {
                            screenBuf.pop_back();
                            userData.save("coins", std::to_string(coins));
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

        switch (screenBuf.back()) {
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
                coinCount.setString(userData.get("coins"));
                window.clear();
                window.draw(background);
                window.draw(play);
                window.draw(shop);
                window.draw(coinCount);
                window.draw(coin);
                window.draw(usernameText);
                window.draw(totalPointsText);
                if (userData.get("offers.starterpack.claimed") != "1") {
                    window.draw(free);
                }
            } break;
            case screens::SHOP: {
                coinCount.setString(userData.get("coins"));
                window.clear();
                window.draw(background);
                if (userData.get("offers.starterpack.claimed") == "0") {
                    window.draw(starterPackOffer);
                }
                window.draw(back);
                window.draw(coinCount);
                window.draw(coin);
            } break;
            case screens::COIN: {
                window.clear();
                gotCoins.setString(std::to_string(coins_last));
                window.draw(coin_sprite);
                window.draw(gotCoins);
            } break;
            case screens::BOX: {
                window.clear();
                window.draw(boxscreen);
            }
        }

        window.display();
    }

    return 0;
}