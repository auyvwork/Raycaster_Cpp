#include <random>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include "map.h"
#include "player.h"
#include "renderer.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(SCREEN_WIDTH), static_cast<unsigned int>(SCREEN_HEIGHT))), "Raycaster C++",
            sf::Style::Close | sf::Style::Titlebar);

    window.setFramerateLimit(120);

    int height = 15;
    int width = 20;

    std::vector<std::vector<int>> grid(height, std::vector<int>(width, 1));

    int dx[] = {0, 0, -2, 2};
    int dy[] = {-2, 2, 0, 0};

    std::mt19937 rng(static_cast<unsigned>(time(nullptr)));

    std::function<void(int, int)> carve = [&](int x, int y) {
            grid[y][x] = 0;

            std::vector<int> dirs = {0, 1, 2, 3};
            std::shuffle(dirs.begin(), dirs.end(), rng);

            for (int i : dirs) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1 && grid[ny][nx] == 1) {
                            grid[y + dy[i] / 2][x + dx[i] / 2] = 0;
                            carve(nx, ny);
                    }
            }
    };

    carve(1, 1);

    float cellSize = 20.0f;
    Map map(cellSize, grid);

    Player player;
    player.position = sf::Vector2f(1.5f * cellSize, 1.5f * cellSize);
    player.angle = 90.0f;

    Renderer renderer;

    sf::Clock gameClock;
    while (window.isOpen()) {
            float deltaTime = gameClock.restart().asSeconds();
            while (auto event = window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                            window.close();
                    }
            }
            window.clear(sf::Color::Black);

            player.update(deltaTime, map);

            renderer.draw3dView(window, player, map);

            map.draw(window);
            player.draw(window);
            renderer.drawRays(window, player, map);

            window.display();
    }
}