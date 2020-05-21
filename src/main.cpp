#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/Pacman.h"
#include "../include/Game_window.h"
#include "../include/Maze.h"
#include "../include/GhostController.h"
#include "../include/PacmanController.h"

using namespace std;

static const std::string PACMAN_TEXTURE = "../resources/pacman.png";
static const std::vector<std::string> GHOST_TEXTURES = {"../resources/blinky.png", "../resources/sue.png",
                                                        "../resources/inky.png", "../resources/pinky.png"};
float GHOST_SPEED = 100.f;
static vector<sf::Vector2f> getGhostsPos(const deque<Ghost>& ghosts) {
    vector<sf::Vector2f> res;
    res.reserve(ghosts.size());
    for (const Ghost& ghost : ghosts) {
        res.push_back(ghost.getPosition());
    }
    return res;
}

void startGame(int level) {
    if (level > 3) {
        return;
    }
    if (level == 3) {
        GHOST_SPEED = 150.f;
    }
    Maze maze(level);
    GameWindow gameWindow(maze.getWidth() * maze.getBlockSize(), maze.getHeight() * maze.getBlockSize(), "PACMAN GAME");
    Pacman pacman(PACMAN_TEXTURE, maze.getBlockSize()/2.f);
    deque<Ghost> ghosts;

    size_t i = 0;
    for (const sf::Vector2f& point : maze.getGhostsInitCoords()) {
        ghosts.emplace_back(GHOST_TEXTURES[i], sf::Vector2f{maze.getBlockSize(), maze.getBlockSize()}, point, GHOST_SPEED);
        i = (i + 1) % GHOST_TEXTURES.size();
        //if (i > 2) break;
    }

    gameWindow.setMaze(&maze);
    gameWindow.setPacman(&pacman);
    gameWindow.setPathAlgorithms();
    gameWindow.setGhosts(&ghosts);

    sf::Clock clock;

    bool lose = false;
    bool win = false;

    int cookiesCnt = maze.getCookies().size();

    while (gameWindow.isOpen() && (!lose && !win)) {
        gameWindow.handleEvents();
        const float elapsedTime = clock.getElapsedTime().asSeconds();
        clock.restart();

        gameWindow.setTitle("PAC-MAN GAME | Fruits: " + to_string(pacman.getCountCookies()) + "/" + to_string(cookiesCnt) + " | Level: " + to_string(level));

        pacman.updatePacman(elapsedTime);

        for (Ghost& ghost : ghosts) {
            ghost.update(elapsedTime);
        }

        for (int i = 0; i < ghosts.size(); ++i) {
            for (int j = 0; j < i; ++j) {
                if (ghosts[i].getGlobalBounds().intersects(ghosts[j].getGlobalBounds())) {
                    ghosts[j].stopMoving();
                }
            }
        }

        for (const Ghost& ghost : ghosts) {
            if (ghost.getGlobalBounds().intersects(pacman.getGlobalBounds())) {
                lose = true;
                break;
            }
        }

        auto& visited = maze.visited;
        for (int x = 0; x < visited.size(); ++x) {
            for (int y = 0; y < visited[x].size(); ++y) {
                visited[x][y] *= 0.95;
            }
        }

        if (pacman.isWaiting()) {
            auto to = PacmanController::GetDecision(pacman, ghosts, gameWindow.getPathAlgorithms());
            pacman.setTarget({to.x * maze.getBlockSize(), to.y * maze.getBlockSize()});
        }


        sf::Vector2f pacManPos = gameWindow.getPathAlgorithms().convertCoordinatesToMaze(pacman.getPosition());
        visited[pacManPos.x][pacManPos.y] += 1;


        maze.setGhostsPositions(getGhostsPos(ghosts), true);
        for (Ghost& ghost : ghosts) {
            if (ghost.isWaiting()) {
                auto to = GhostController::GetDecision(pacman, ghost, gameWindow.getPathAlgorithms());
                ghost.setTarget({to.x * maze.getBlockSize(), to.y *maze.getBlockSize()});
            }
        }

        if (maze.getCellbyPosition(maze.castCoordinatesForCellPosition(pacman.getPosition())).isCookie() ||
            maze.getCellbyPosition(maze.castCoordinatesForCellPosition(pacman.getPosition())).isFruit()) {
            maze.eatCookie(const_cast<Cell &>(maze.getCellbyPosition(
                    maze.castCoordinatesForCellPosition(pacman.getPosition()))));
            pacman.eatCookies();
            if (maze.getCookies().empty()) {
                win = true;
            }
        }

        maze.setGhostsPositions(getGhostsPos(ghosts), false);

        gameWindow.render();
    }

    if (lose) {
        std::cout << "PACMAN LOSE\n";
    } else if (win) {
        std::cout << "PACMAN WON \n";
        startGame(level + 1);
    } else {
        std::cout << "EXIT\n";
    }
    std::cout << "Pacman has eaten " << pacman.getCountCookies() << " fruits\n";
}

int main() {
    startGame(2);
    return 0;
}