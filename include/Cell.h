#pragma once

#include <SFML/Graphics.hpp>

enum class CellType {
    WALL,
    ROAD,
    COOKIE,
    FRUIT,
    UNDEFINED
};

class Cell : public sf::RectangleShape{
public:
    Cell();
    bool isWall() const;
    bool isCookie() const;
    bool isRoad() const;
    bool isFruit() const;
    float getBlockSize() const;
    void setCellType(const CellType& cellType);
    CellType getCellType() const;
    sf::CircleShape getCookieShape() const;
    bool getGhost() const;
    void setGhost(bool value);
private:
    sf::CircleShape cookieShape;
    const float blockSize = 25.f;
    CellType cellType;
    bool isGhost = false;
};


