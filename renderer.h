#ifndef _RENDERER_H
#define _RENDERER_H
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "map.h"
#include "player.h"

constexpr float SCREEN_WIDTH = 1200.0f;
constexpr float SCREEN_HEIGHT = 675.0f;

class Renderer {
public:
    void draw3dView(sf::RenderTarget &target, const Player &player, const Map &map);
    void drawRays(sf::RenderTarget &target, const Player &player, const Map &map);
};

#endif