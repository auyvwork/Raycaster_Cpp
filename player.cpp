#include "player.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cmath>

constexpr float PI = 3.1415926535f;

constexpr float MAX_TURN_SPEED = 170.0f;
constexpr float TURN_ACCEL = 1500.0f;
constexpr float TURN_FRICTION = 2500.0f;

constexpr float MAX_SPEED = 50.0f;
constexpr float ACCELERATION = 800.0f;
constexpr float FRICTION = 1200.0f;

void Player::draw(sf::RenderTarget &target) const {
    sf::CircleShape circle(5.f);
    circle.setPosition(position);
    circle.setOrigin(sf::Vector2f(circle.getRadius(), circle.getRadius()));
    circle.setFillColor(sf::Color::Blue);

    sf::RectangleShape line(sf::Vector2f(24.0f, 2.0f));
    line.setPosition(position);
    line.setRotation(sf::degrees(angle));
    line.setFillColor(sf::Color::Blue);

    target.draw(line);
    target.draw(circle);
}


float getLength(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

void Player::update(float deltaTime, const Map& map) {

    float targetTurn = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        targetTurn = -MAX_TURN_SPEED;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        targetTurn = MAX_TURN_SPEED;
    }


    if (turnVelocity < targetTurn) {
        turnVelocity += TURN_ACCEL * deltaTime;
        if (turnVelocity > targetTurn && targetTurn < 0) turnVelocity = targetTurn;
    } else if (turnVelocity > targetTurn) {
        turnVelocity -= TURN_ACCEL * deltaTime;
        if (turnVelocity < targetTurn && targetTurn > 0) turnVelocity = targetTurn;
    }


    if (targetTurn == 0.0f) {
        if (turnVelocity > 0) {
            turnVelocity -= TURN_FRICTION * deltaTime;
            if (turnVelocity < 0) turnVelocity = 0;
        } else if (turnVelocity < 0) {
            turnVelocity += TURN_FRICTION * deltaTime;
            if (turnVelocity > 0) turnVelocity = 0;
        }
    }

    angle += turnVelocity * deltaTime;

    if (angle < 0) angle += 360;
    if (angle >= 360) angle -= 360;

    float radians = angle * PI / 180.0f;
    sf::Vector2f direction(cos(radians), sin(radians));
    sf::Vector2f accelerationVec(0.0f, 0.0f);
    bool isMoving = false;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        accelerationVec += direction * ACCELERATION;
        isMoving = true;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        accelerationVec -= direction * ACCELERATION;
        isMoving = true;
    }

    velocity += accelerationVec * deltaTime;

    if (!isMoving) {
        float currentSpeed = getLength(velocity);
        if (currentSpeed > 0) {
            float drop = FRICTION * deltaTime;
            float newSpeed = currentSpeed - drop;
            if (newSpeed < 0) newSpeed = 0;

            velocity /= currentSpeed;
            velocity *= newSpeed;
        }
    } else {
        float currentSpeed = getLength(velocity);
        if (currentSpeed > MAX_SPEED) {
            velocity /= currentSpeed;
            velocity *= MAX_SPEED;
        }
    }

    sf::Vector2f moveStep = velocity * deltaTime;

    if (getLength(moveStep) < 0.001f) return;

    float cellSize = map.getCellSize();
    const auto& grid = map.getGrid();

    sf::Vector2f nextPos = position;
    nextPos.x += moveStep.x;

    int gridX = static_cast<int>(nextPos.x / cellSize);
    int gridY = static_cast<int>(position.y / cellSize);

    if (gridY >= 0 && gridY < grid.size() && gridX >= 0 && gridX < grid[0].size()) {
        if (grid[gridY][gridX] == 0) {
            position.x = nextPos.x;
        } else {
            velocity.x = 0;
        }
    }

    nextPos = position;
    nextPos.y += moveStep.y;

    gridX = static_cast<int>(position.x / cellSize);
    gridY = static_cast<int>(nextPos.y / cellSize);

    if (gridY >= 0 && gridY < grid.size() && gridX >= 0 && gridX < grid[0].size()) {
        if (grid[gridY][gridX] == 0) {
            position.y = nextPos.y;
        } else {
            velocity.y = 0;
        }
    }
}