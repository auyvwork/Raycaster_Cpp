#pragma once
#ifndef Player_h
#define Player_H_

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include "map.h"

class Player {
public:
    Player() : position(0, 0), angle(0), velocity(0, 0), turnVelocity(0) {}

    void draw(sf::RenderTarget &target) const;

    void update(float deltaTime, const Map& map);

    sf::Vector2f position;
    float angle;

    sf::Vector2f velocity;
    float turnVelocity;
};
#endif