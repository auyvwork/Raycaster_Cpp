#pragma once
#ifndef Player_h
#define Player_H_
#include <SFML/Graphics/RenderTarget.hpp>


class Player {
    public:
        void draw(sf::RenderTarget &target) const;
        void update(float deltaTime);
        sf::Vector2f position;
        float angle;
};

#endif
