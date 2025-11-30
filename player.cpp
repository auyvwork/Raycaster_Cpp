#include "player.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>

constexpr  float TURN_SPEED = 200.0F;
constexpr float MOVE_SPEED = 30.0F;
constexpr float PI =  3.1415926535f;

void Player::draw(sf::RenderTarget &target) const {
    sf::CircleShape circle(5.f);
    circle.setPosition(position);
    circle.setOrigin(sf::Vector2f(circle.getRadius(),circle.getRadius()));
    circle.setFillColor(sf::Color::Blue);

    sf::RectangleShape line(sf::Vector2f(24.0f,2.0f));
    line.setPosition(position);
    line.setRotation(sf::degrees(angle));
    line.setFillColor(sf::Color::Blue);

    target.draw(line);


    target.draw(circle);

}

void Player::update(float deltaTime) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        angle -= TURN_SPEED * deltaTime;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        angle += TURN_SPEED * deltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        float radians = angle * PI / 180;

        position.x += cos(radians)*deltaTime *MOVE_SPEED;
        position.y += sin(radians)*deltaTime *MOVE_SPEED;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        float radians = angle * PI / 180;

        position.x -= cos(radians) * deltaTime * MOVE_SPEED;
        position.y -= sin(radians) * deltaTime * MOVE_SPEED;
    }
}