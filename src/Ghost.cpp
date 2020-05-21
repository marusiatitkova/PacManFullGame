#include <math.h>
#include "../include/Ghost.h"

Ghost::Ghost(const std::string &pathToTexture, const sf::Vector2f &size, const sf::Vector2f &coord, float speed)
                        : sf::RectangleShape(size), kSpeed(speed){
    setPosition(coord);
    texture.loadFromFile(pathToTexture);
    setTexture(&texture);
    isWaiting_ = true;
    targetPoint = {-1, -1};
}

void Ghost::update(float elapsedTime) {
//    if (targetPoint == sf::Vector2f{-1, -1}) {
//        return;
//    }
//
//    float step = kSpeed * elapsedTime;
//    sf::Vector2f currentPosition = {getPosition().x, getPosition().y};
//    sf::Vector2f direction = targetPoint - currentPosition;
//    float distance = sqrtf((direction.x * direction.x) + (direction.y * direction.y));
//    sf::Vector2f unitVector(direction.x / distance, direction.y / distance);
//    if (distance < 1.f) {
//        checkTarget(currentPosition + direction);
//        move (direction);
//    } else {
//        checkTarget(currentPosition + unitVector * step);
//        move(unitVector * step);
//    }
    if (targetPoint == sf::Vector2f{-1, -1}) {
        return;
    }

    const float step = kSpeed * elapsedTime;

    float x = getPosition().x;
    float y = getPosition().y;
    float newX = x;
    float newY = y;

    if (x < targetPoint.x) {
        if (x + step > targetPoint.x) {
            newX = targetPoint.x;
        } else {
            newX = x + step;
        }
    } else if (newX > targetPoint.x) {
        if (newX - step < targetPoint.x) {
            newX = targetPoint.x;
        } else {
            newX = x - step;
        }
    } else if (y < targetPoint.y) {
        if (y + step > targetPoint.y) {
            newY = targetPoint.y;
        } else {
            newY = y + step;
        }
    } else if (y > targetPoint.y) {
        if (y - step < targetPoint.y) {
            newY = targetPoint.y;
        } else {
            newY = y - step;
        }
    }

    if (newX == targetPoint.x && newY == targetPoint.y) {
        isWaiting_ = true;
    }

    setPosition(newX, newY);
}

void Ghost::checkTarget(const sf::Vector2f& newPosition) {
    if (newPosition == targetPoint) {
      isWaiting_ = true;
    }
}

bool Ghost::isWaiting() const {
    return isWaiting_;
}

void Ghost::setTarget(sf::Vector2f target) {
    targetPoint = target;
    isWaiting_ = false;
}

void Ghost::stopMoving() {
    isWaiting_ = true;
    targetPoint = {-1, -1};
}


