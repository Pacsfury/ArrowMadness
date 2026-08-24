#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

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

    std::vector<int> balls = {1, 3, 2};

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
                        auto it = std::find(balls.begin(), balls.end(), 1);

                        if (it != balls.end()) {
                            balls.erase(it);
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    } 
                    if (down.getGlobalBounds().contains(mousePos)) {
                        down.setColor(clicked);
                        auto it = std::find(balls.begin(), balls.end(), 2);

                        if (it != balls.end()) {
                            balls.erase(it);
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    }
                    if (left.getGlobalBounds().contains(mousePos)) {
                        left.setColor(clicked);
                        auto it = std::find(balls.begin(), balls.end(), 3);

                        if (it != balls.end()) {
                            balls.erase(it);
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 1));
                        } else {
                            points.setString("0");
                        }
                    }
                    if (right.getGlobalBounds().contains(mousePos)) {
                        right.setColor(clicked);
                        auto it = std::find(balls.begin(), balls.end(), 4);

                        if (it != balls.end()) {
                            balls.erase(it);
                            points.setString(std::to_string(std::stoi(points.getString().toAnsiString()) + 4));
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

        window.clear();
        window.draw(background);
        window.draw(up);
        window.draw(down);
        window.draw(right);
        window.draw(left);
        window.draw(points);
        window.display();
    }

    return 0;
}
