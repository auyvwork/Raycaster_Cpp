#include "renderer.h"

#include <SFML/Graphics/RectangleShape.hpp>

constexpr float PI =  3.1415926535f;

constexpr float PLAYER_FOV = 120;
constexpr size_t MAX_RAYCASTING_STEPS = 64;
constexpr size_t NUM_RAYS = 1200;

constexpr float SCREEN_HEIGHT = 675.0f;
constexpr float COLUMN_WIDTH = SKREEN_w / (float)NUM_RAYS;

struct Ray {
    sf::Vector2f hitPosition;
    float distance;
    bool hit;
    bool isHitVertical;
};
static Ray castRay(sf::Vector2f start,float angleInDegrees,const Map &map);


void Renderer::draw3dView(sf::RenderTarget &target, const Player &player, const Map &map) {
    float angle = player.angle - PLAYER_FOV / 2.0f;
    float angleIncrement = PLAYER_FOV / static_cast<float>(NUM_RAYS);

    for (size_t i = 0; i < NUM_RAYS; i++, angle += angleIncrement) {
        Ray ray = castRay(player.position, angle, map);

        if (ray.hit) {

            float correctedDistance = ray.distance * cos((angle - player.angle) / 1.20f * PI / 180.0f);

            float wallH = (map.getCellSize() * SCREEN_HEIGHT) / correctedDistance;


            if (wallH > SCREEN_HEIGHT) wallH = SCREEN_HEIGHT;

            sf::RectangleShape column(sf::Vector2f(COLUMN_WIDTH, wallH));
            float shade =  ray.isHitVertical ? 0.8f : 1.0f;
            float shade2 = ray.distance ;
            float shape3 = 255 - 2*(shade * shade2 * 0.2f) * (shade * shade2 * 0.2f);

            if (shape3 < 0) {
                shape3 = 0;
            }
            else if (shape3 > 255) {
                shape3 = 255;

            }
            column.setFillColor(sf::Color( shape3 ,shape3 , shape3 ));


            column.setPosition(sf::Vector2f(i * COLUMN_WIDTH, (SCREEN_HEIGHT - wallH) / 2.0f));

            target.draw(column);
        }
    }
}
void  Renderer::drawRays(sf::RenderTarget &target, const Player &player,const Map &map) {


    for (float angle = player.angle - PLAYER_FOV / 2.0f; angle < player.angle + PLAYER_FOV; angle+= 0.1f) {
        Ray ray = castRay(player.position, angle,map);

        if (ray.hit) {
            sf::Vertex vertices[2];
            vertices[0].position = player.position;
            vertices[0].color = sf::Color::Red;
            vertices[1].position = ray.hitPosition;
            vertices[1].color = sf::Color::Red;
            target.draw(vertices, 2, sf::PrimitiveType::Lines);
        }
    }

}

Ray castRay(sf::Vector2f start,float angleInDegrees, const Map &map) {
     float angle = angleInDegrees * PI / 180;
    float vTan = -tan(angle), hTan = -1.0f / tan(angle);
    float cellSize = map.getCellSize();
    bool Hit = false;
    size_t vdof = 0, hdof = 0;
    float hdist = std::numeric_limits<float>::max();
    float vdist = std::numeric_limits<float>::max();

    sf::Vector2f vRayPos, hRayPos,offset;




    if (cos(angle) > 0.001f) {
        vRayPos.x = std::floor(start.x / cellSize)*cellSize + cellSize;
        vRayPos.y = (start.x - vRayPos.x) * vTan + start.y;
        offset.x = cellSize;
        offset.y = -offset.x *vTan;
    }else if (cos(angle) < -0.001f) {
        vRayPos.x = std::floor(start.x / cellSize) * cellSize - 0.01f;
        vRayPos.y = (start.x - vRayPos.x) * vTan +start.y;
        offset.x = -cellSize;
        offset.y = -offset.x *vTan;
    }else {
        vdof = MAX_RAYCASTING_STEPS;
    }


    const auto &grid = map.getGrid();
    for (; vdof < MAX_RAYCASTING_STEPS; vdof++) {
        int mapX = static_cast<int>(vRayPos.x / cellSize);
        int mapY = static_cast<int>(vRayPos.y / cellSize);

        if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {

            Hit = true;
            vdist = std::sqrt((vRayPos.x - start.x) * (vRayPos.x - start.x)
            + (vRayPos.y - start.y) * (vRayPos.y - start.y));

            break;
        }
        vRayPos += offset;
    }








    if (sin(angle) > 0.001f) {
        hRayPos.y = std::floor(start.y / cellSize)*cellSize + cellSize;
        hRayPos.x = (start.y - hRayPos.y) * hTan +start.x;
        offset.y = cellSize;
        offset.x = -offset.y *hTan;
    }else if (sin(angle) < -0.001f) {
        hRayPos.y = std::floor(start.y / cellSize)*cellSize - 0.01f;
        hRayPos.x = (start.y - hRayPos.y) * hTan +start.x;
        offset.y = -cellSize;
        offset.x = -offset.y *hTan;
    }else {
        hdof = MAX_RAYCASTING_STEPS;
    }

    for (; hdof < MAX_RAYCASTING_STEPS; hdof++) {
        int mapX = static_cast<int>(hRayPos.x / cellSize);
        int mapY = static_cast<int>(hRayPos.y / cellSize);

        if (mapY < grid.size() && mapX < grid[mapY].size() && grid[mapY][mapX]) {
            Hit = true;
            hdist = std::sqrt((hRayPos.x - start.x) * (hRayPos.x - start.x)
           + (hRayPos.y - start.y) * (hRayPos.y - start.y));
            break;
        }
        hRayPos += offset;
    }

    return Ray{hdist < vdist ? hRayPos : vRayPos, std::min(hdist,vdist), Hit ,vdist< hdist };





}