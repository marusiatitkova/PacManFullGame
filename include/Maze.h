#pragma once

#include "Cell.h"
#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>

static const char WALL_MARKER = '#';
static const char ROAD_MARKER = ' ';
static const char COOKIE_MARKER = '.';
static const char PACMAN_MARKER = 'P';
static const char GHOST_MARKER = 'G';
static const char CHERRY_MARKER = 'C';

static const float COOKIE_RADIUS = 3.f;
static const float FRUIT_RADIUS = 12.f;
static const sf::Color COOKIE_COLOR = sf::Color(255, 229, 204);

static const std::vector<std::string> FIELD_MAZE_LEVEL_TWO = {{" ####################### "},
                                                              {" #..........#..........# "},
                                                              {" #.##.#####.#.#####.##.# "},
                                                              {" #.....................# "},
                                                              {" #.##.#.#########.#.##.# "},
                                                              {" #....#.....#..C..#....# "},
                                                              {" ####.#####.#.#####C#### "},
                                                              {"    #.#...........#.#    "},
                                                              {"#####.#.#.##.##.#.#.#####"},
                                                              {"#.......#.#GGG#.# ......#"},
                                                              {"#####.#.#.#####.#.#.#####"},
                                                              {"    #.#...........#.#    "},
                                                              {" ####.#.#########.#.#### "},
                                                              {" #..........#..........# "},
                                                              {" #.##.#####.#.#####.##.# "},
                                                              {" #.C#.......P.......#..# "},
                                                              {" ##.#.#.#########.#.#.C# "},
                                                              {" #....#.....#.....#....# "},
                                                              {" #.#######.###.#######.# "},
                                                              {" #.#...C.#.....#.....#C# "},
                                                              {" #.#.###.##.#.##.###.#.# "},
                                                              {" #.#.#......#......#.#.# "},
                                                              {" #.#.#.####.#.####.#.#.# "},
                                                              {" #..........#....CC....# "},
                                                              {" ####################### "}};

static const std::vector<std::string> FIELD_MAZE_LEVEL_ONE = {{"############################"},
                                                              {"#............##............#"},
                                                              {"#.####.#####.##.#####.####.#"},
                                                              {"#G#  #.#   #.##.#   #.#  #G#"},
                                                              {"#.####.#####.##.#####.####.#"},
                                                              {"#..........................#"},
                                                              {"#.####.##.########.##.####.#"},
                                                              {"#.####.##.########.##.####.#"},
                                                              {"#......##....##....##......#"},
                                                              {"######.#####.##.#####.######"},
                                                              {"     #.#####.##.#####.#     "},
                                                              {"     #.##C.........##.#     "},
                                                              {"     #.##.###.P###.##.#     "},
                                                              {"######.##.#......#.##.######"},
                                                              {"##........#.C....#........##"},
                                                              {"######.##.#...C..#.##.######"},
                                                              {"     #.##C########.##.#     "},
                                                              {"     #.##.........C##.#     "},
                                                              {"     #.##.########.##.#     "},
                                                              {"######.##.########.##.######"},
                                                              {"#............##............#"},
                                                              {"#.####.#####.##.#####.####.#"},
                                                              {"#.####.#####.##.#####.####.#"},
                                                              {"#G..##.......CC.......##..G#"},
                                                              {"###.##.##.########.##.##.###"},
                                                              {"###.##.##.########.##.##.###"},
                                                              {"#......##....##....##......#"},
                                                              {"#.##########.##.##########.#"},
                                                              {"#.##########.##.##########.#"},
                                                              {"#..........................#"},
                                                              {"############################"}};

class Maze {
public:
    Maze(int level);
    size_t getWidth() const;
    size_t getHeight() const;
    float getBlockSize() const;

    const std::vector<std::vector<Cell>>& getCells() const;
    void setCells();


    const sf::Vector2f &getPacmanPosition() const;
    std::vector<sf::CircleShape> getCookies() const;
    std::vector<CellType > getCookieType() const;
    void eatCookie(Cell& cell);

    Cell& getCellbyPosition(const sf::Vector2f& pos);
    sf::Vector2f castCoordinatesForCellPosition(const sf::Vector2f& pos);

    const std::vector<sf::Vector2f>& getGhostsInitCoords() const;
    void setGhostsPositions(std::vector<sf::Vector2f> ghostsPos, bool value = true);

    std::vector<std::vector<double>> visited;
private:
    size_t width{0};
    size_t height{0};
    std::vector<std::string> field_maze;
    std::vector<std::vector<Cell>> cells;
    sf::Vector2f pacmanPosition;
    std::vector<sf::Vector2f> ghostsInitCoords;
    const float blockSize = Cell().getBlockSize();
    std::vector<sf::CircleShape> cookies;
    std::vector<CellType> cookieType;
    sf::Texture cherryTexture;
    sf::Vector2f getCellPositionFromCenter(const sf::Vector2f &center, const CellType &cell);
};


