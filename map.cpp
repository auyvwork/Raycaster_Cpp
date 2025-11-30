#include "map.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

Map::Map(float cellSize, int width, int height)
    : cellSize(cellSize), grid(height, std::vector<int>(width, 0)) {}
Map::Map(float cellSize, std::vector<std::vector<int>> grid)
    : cellSize(cellSize), grid(grid) {}

void Map::draw(sf::RenderTarget &target) {
    if (grid.empty()) {
        return;
    }

    sf::RectangleShape background(sf::Vector2f(static_cast<int>(grid[0].size() * cellSize), static_cast<int>(grid.size() * cellSize)));
    background.setFillColor(sf::Color::Green);
    target.draw(background);
    sf::RectangleShape cell(sf::Vector2f(cellSize * 0.95f, cellSize * 0.95f ));

    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[0].size(); x++) {
            if (grid[y][x] == 0) {
                cell.setFillColor(sf::Color::White);
            }
            else if (grid[y][x] == 1) {
                cell.setFillColor(sf::Color::Black);

            }
            cell.setPosition(sf::Vector2f(x, y) * cellSize + sf::Vector2f(0.025f * cellSize, 0.025f * cellSize));
            target.draw(cell);
        }
    }
}


const std::vector<std::vector<int>> &Map::getGrid() const { return grid; }

float Map::getCellSize() const {return cellSize;}
