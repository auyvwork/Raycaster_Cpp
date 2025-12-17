#include "renderer.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include <algorithm>
#include <limits>


constexpr float PI = 3.1415926535f;
constexpr float PLAYER_FOV = 120.0f;
constexpr size_t MAX_RAYCASTING_STEPS = 200;
constexpr size_t NUM_RAYS = 1200;
constexpr float COLUMN_WIDTH = SCREEN_WIDTH / (float)NUM_RAYS;

struct Ray {
    sf::Vector2f hitPosition;
    float distance;
    bool hit;
    bool isHitVertical;
};

static Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map);

void Renderer::draw3dView(sf::RenderTarget &target, const Player &player, const Map &map) {

    sf::RectangleShape ceiling(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f));
    ceiling.setFillColor(sf::Color(50, 50, 100)); // Темно-сине-серый
    ceiling.setPosition(sf::Vector2f(0,0));
    target.draw(ceiling);

    sf::RectangleShape floor(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT / 2.0f));
    floor.setFillColor(sf::Color(20, 20, 20)); // Серый
    floor.setPosition(sf::Vector2f(0, SCREEN_HEIGHT / 2.0f));
    target.draw(floor);

    float angleStart = player.angle - PLAYER_FOV / 2.0f;
    float angleIncrement = PLAYER_FOV / static_cast<float>(NUM_RAYS);
    float angle = angleStart;

    for (size_t i = 0; i < NUM_RAYS; i++, angle += angleIncrement) {
        Ray ray = castRay(player.position, angle, map);

        if (ray.hit) {
            float correctedDistance = ray.distance * cos((angle - player.angle) * PI / 180.0f);

            if (correctedDistance < 0.1f) correctedDistance = 0.1f;

            float wallH = (map.getCellSize() * SCREEN_HEIGHT) / correctedDistance;

            float drawHeight = wallH;
            if (drawHeight > SCREEN_HEIGHT) drawHeight = SCREEN_HEIGHT;

            sf::RectangleShape column(sf::Vector2f(COLUMN_WIDTH, drawHeight));

            column.setPosition(sf::Vector2f(i * COLUMN_WIDTH, (SCREEN_HEIGHT - drawHeight) / 2.0f));

            float intensity = 255.0f / (1.0f + correctedDistance * 0.05f);
            if (ray.isHitVertical) intensity *= 0.9f;

            if (intensity > 255.0f) intensity = 255.0f;
            if (intensity < 0.0f) intensity = 0.0f;

            unsigned char colorVal = static_cast<unsigned char>(intensity);

            column.setFillColor(sf::Color(colorVal, colorVal, colorVal));
            target.draw(column);
        }
    }
}

void Renderer::drawRays(sf::RenderTarget &target, const Player &player, const Map &map) {
    float angleIncrement = PLAYER_FOV / 50.0f;

    for (float angle = player.angle - PLAYER_FOV / 2.0f; angle < player.angle + PLAYER_FOV / 2.0f; angle += angleIncrement) {
        Ray ray = castRay(player.position, angle, map);
        if (ray.hit) {
            sf::Vertex vertices[2];
            vertices[0].position = player.position;
            vertices[0].color = sf::Color(255, 0, 0, 100);
            vertices[1].position = ray.hitPosition;
            vertices[1].color = sf::Color(255, 0, 0, 100);
            target.draw(vertices, 2, sf::PrimitiveType::Lines);
        }
    }
}

Ray castRay(sf::Vector2f start, float angleInDegrees, const Map &map) {
    float angle = angleInDegrees * PI / 180.0f;

    while (angle < 0) angle += 2 * PI;
    while (angle >= 2 * PI) angle -= 2 * PI;

    float cellSize = map.getCellSize();

    float hDist = std::numeric_limits<float>::max();
    sf::Vector2f hHitPos;
    bool hHit = false;

    float aTan = -1.0f / tan(angle);
    sf::Vector2f rayPos, offset;

    if (angle > PI) {
        rayPos.y = std::floor(start.y / cellSize) * cellSize - 0.0001f;
        rayPos.x = (start.y - rayPos.y) * aTan + start.x;
        offset.y = -cellSize;
        offset.x = -offset.y * aTan;
    } else if (angle < PI && angle > 0) {
        rayPos.y = std::floor(start.y / cellSize) * cellSize + cellSize;
        rayPos.x = (start.y - rayPos.y) * aTan + start.x;
        offset.y = cellSize;
        offset.x = -offset.y * aTan;
    } else {
        hDist = std::numeric_limits<float>::max();
    }

    if (angle != 0 && angle != PI) {
        const auto &grid = map.getGrid();
        for (int i = 0; i < MAX_RAYCASTING_STEPS; i++) {
            int mx = static_cast<int>(rayPos.x / cellSize);
            int my = static_cast<int>(rayPos.y / cellSize);

            if (my >= 0 && my < grid.size() && mx >= 0 && mx < grid[0].size()) {
                if (grid[my][mx] == 1) {
                    hHit = true;
                    hHitPos = rayPos;
                    hDist = std::sqrt(pow(hHitPos.x - start.x, 2) + pow(hHitPos.y - start.y, 2));
                    break;
                }
            } else {
                break;
            }
            rayPos += offset;
        }
    }

    float vDist = std::numeric_limits<float>::max();
    sf::Vector2f vHitPos;
    bool vHit = false;

    float nTan = -tan(angle);

    if (angle > PI / 2 && angle < 3 * PI / 2) {
        rayPos.x = std::floor(start.x / cellSize) * cellSize - 0.0001f;
        rayPos.y = (start.x - rayPos.x) * nTan + start.y;
        offset.x = -cellSize;
        offset.y = -offset.x * nTan;
    } else if (angle < PI / 2 || angle > 3 * PI / 2) {
        rayPos.x = std::floor(start.x / cellSize) * cellSize + cellSize;
        rayPos.y = (start.x - rayPos.x) * nTan + start.y;
        offset.x = cellSize;
        offset.y = -offset.x * nTan;
    } else {
        vDist = std::numeric_limits<float>::max();
    }

    if (angle != PI / 2 && angle != 3 * PI / 2) {
        const auto &grid = map.getGrid();
        for (int i = 0; i < MAX_RAYCASTING_STEPS; i++) {
            int mx = static_cast<int>(rayPos.x / cellSize);
            int my = static_cast<int>(rayPos.y / cellSize);

            if (my >= 0 && my < grid.size() && mx >= 0 && mx < grid[0].size()) {
                if (grid[my][mx] == 1) {
                    vHit = true;
                    vHitPos = rayPos;
                    vDist = std::sqrt(pow(vHitPos.x - start.x, 2) + pow(vHitPos.y - start.y, 2));
                    break;
                }
            } else {
                break;
            }
            rayPos += offset;
        }
    }

    if (hDist < vDist) {
        return {hHitPos, hDist, hHit, false};
    } else {
        return {vHitPos, vDist, vHit, true};
    }
}