#include "../include/Maze.h"

static CellType getCellType(char ch) {
    switch (ch) {
        case WALL_MARKER:
            return CellType::WALL;
        case ROAD_MARKER:
            return CellType::ROAD;
        case PACMAN_MARKER:
            return CellType::ROAD;
        case GHOST_MARKER:
            return CellType::ROAD;
        case COOKIE_MARKER:
            return CellType::COOKIE;
        case CHERRY_MARKER:
            return CellType::FRUIT;
        default:
            std::cerr << "Cell type not recognised\n";
            return CellType ::UNDEFINED;
    }
}

Maze::Maze(int level) {
    if (level == 1) {
        field_maze = FIELD_MAZE_LEVEL_ONE;
    } else
        field_maze = FIELD_MAZE_LEVEL_TWO;
    width = field_maze[0].size();
    height = field_maze.size();
    cherryTexture.loadFromFile("../resources/fruit.png");
    visited = std::vector<std::vector<double>>(width, std::vector<double>(height, 0.0f));
    setCells();
}

size_t Maze::getWidth() const {
    return width;
}

size_t Maze::getHeight() const {
    return height;
}

const std::vector<std::vector<Cell>> &Maze::getCells() const {
    return cells;
}

void Maze::setCells() {
    cells.resize(width);
    for (size_t x = 0; x < width; x++) {
        cells[x].resize(height);
        for (size_t y = 0; y < height; y++) {
            cells[x][y].setPosition(x * blockSize, y * blockSize);
            cells[x][y].setCellType(getCellType(field_maze[y][x]));
            if (cells[x][y].isWall()){
                cells[x][y].setFillColor(sf::Color(0, 0, 255));
                //cells[x][y].setFillColor(sf::Color(52, 93, 199));
            }
            if (field_maze[y][x] == GHOST_MARKER) {
                ghostsInitCoords.push_back({x * blockSize, y * blockSize});
            }
            if (field_maze[y][x] == PACMAN_MARKER) {
                pacmanPosition = {x * blockSize, y * blockSize};
            }
            const sf::Vector2f center = cells[x][y].getPosition()
                                        + sf::Vector2f(0.5f * blockSize, 0.5f * blockSize);
            if (cells[x][y].isFruit()) {
                cookies.push_back(cells[x][y].getCookieShape());
                cookieType.push_back(cells[x][y].getCellType());
                cookies.back().setRadius(FRUIT_RADIUS);
                cookies.back().setTexture(&cherryTexture);
                cookies.back().setPosition({center.x - FRUIT_RADIUS , center.y - FRUIT_RADIUS});
            }
            if (cells[x][y].isCookie()) {
                cookies.push_back(cells[x][y].getCookieShape());
                cookieType.push_back(cells[x][y].getCellType());
                cookies.back().setRadius(COOKIE_RADIUS);
                cookies.back().setFillColor(COOKIE_COLOR);
                cookies.back().setPosition({center.x - COOKIE_RADIUS , center.y - COOKIE_RADIUS});
            }
        }
    }
}

float Maze::getBlockSize() const {
    return blockSize;
}

const sf::Vector2f &Maze::getPacmanPosition() const {
    return pacmanPosition;
}

Cell& Maze::getCellbyPosition(const sf::Vector2f& pos) {
    int x = pos.x / blockSize;
    int y = pos.y / blockSize;
    return cells[x][y];
}


sf::Vector2f Maze::castCoordinatesForCellPosition(const sf::Vector2f& pos) {
    sf::Vector2f res = {pos.x / blockSize, pos.y / blockSize};
    res.x = static_cast<int>(res.x);
    res.y = static_cast<int>(res.y);
    return {res.x * blockSize, res.y * blockSize};
}

std::vector<sf::CircleShape> Maze::getCookies() const {
    return cookies;
}

void Maze::eatCookie(Cell& cell) {
    const CellType oldType = cell.getCellType();
    cell.setCellType(CellType::ROAD);
    auto pos = std::find_if(cookies.begin(), cookies.end(), [&](const sf::CircleShape& circle) {
        return getCellPositionFromCenter(circle.getPosition(), oldType) == cell.getPosition();
    });
    cookies.erase(pos);
    int index = std::distance(cookies.begin(), pos);
    cookieType.erase(cookieType.begin() + index);
}

sf::Vector2f Maze::getCellPositionFromCenter(const sf::Vector2f& center, const CellType & cell) {
    float radius = COOKIE_RADIUS;
    if (cell == CellType::FRUIT) {
        radius = FRUIT_RADIUS;
    }
    return {sf::Vector2f(center.x + radius, center.y + radius) - sf::Vector2f(0.5f * blockSize, 0.5f * blockSize)};
}

const std::vector<sf::Vector2f>& Maze::getGhostsInitCoords() const {
    return ghostsInitCoords;
}

void Maze::setGhostsPositions(std::vector<sf::Vector2f> ghostsPos, bool value) {
    for (const sf::Vector2f& point : ghostsPos) {
        getCellbyPosition(point).setGhost(value);
    }
}

std::vector<CellType> Maze::getCookieType() const {
    return cookieType;
}
