#include "../include/Cell.h"

Cell::Cell() {
    setSize(sf::Vector2f{blockSize, blockSize});
    setFillColor(sf::Color(40, 40, 40));
    cellType = CellType ::UNDEFINED;
}

float Cell::getBlockSize() const {
    return blockSize;
}

bool Cell::isWall() const {
    return this->cellType == CellType ::WALL;
}

bool Cell::isCookie() const {
    return this->cellType == CellType ::COOKIE;
}

bool Cell::isRoad() const {
    return this->cellType == CellType ::ROAD;
}

bool Cell::isFruit() const {
    return this->cellType == CellType ::FRUIT;
}

void Cell::setCellType(const CellType& cellType) {
    this->cellType = cellType;
}

sf::CircleShape Cell::getCookieShape() const{
    return cookieShape;
}

CellType Cell::getCellType() const{
    return this->cellType;
}

bool Cell::getGhost() const{
    return isGhost;
}

void Cell::setGhost(bool value) {
    isGhost = value;
}





