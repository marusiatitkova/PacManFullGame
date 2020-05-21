#include "../include/Pacman.h"



sf::CircleShape Pacman::createTarget(const float& radius) {
    sf::CircleShape targetShape(radius);
    targetShape.setFillColor(sf::Color::Green);
    targetShape.setPosition(endTarget);
    return targetShape;
}

void Pacman::updatePacman(float elapsedTime) {
//    if (target == sf::Vector2f{-1, -1}) {
//        return;
//    }
//
//    float step = speed * elapsedTime;
//    sf::Vector2f currentPosition = {getPosition().x, getPosition().y};
//    sf::Vector2f direction = target - currentPosition;
//    float distance = sqrtf((direction.x * direction.x) + (direction.y * direction.y));
//    sf::Vector2f unitVector(direction.x / distance, direction.y / distance);
//    if (distance < 1.f) {
//        checkTarget(currentPosition + direction);
//        move (direction);
//    } else {
//        checkTarget(currentPosition + unitVector * step);
//        move(unitVector * step);
//    }
    if (target == sf::Vector2f{-1, -1}) {
        return;
    }

    const float step = speed * elapsedTime;

    float x = getPosition().x;
    float y = getPosition().y;
    float newX = x;
    float newY = y;

    if (x < target.x) {
        if (x + step > target.x) {
            newX = target.x;
        } else {
            newX = x + step;
        }
    } else if (newX > target.x) {
        if (newX - step < target.x) {
            newX = target.x;
        } else {
            newX = x - step;
        }
    } else if (y < target.y) {
        if (y + step > target.y) {
            newY = target.y;
        } else {
            newY = y + step;
        }
    } else if (y > target.y) {
        if (y - step < target.y) {
            newY = target.y;
        } else {
            newY = y - step;
        }
    }

    if (newX == target.x && newY == target.y) {
//        if (!path.empty()) {
//            target = path.front();
//            path.erase(path.begin());
//            isWaiting_ = false;
//        } else {
            target = {-1, -1};
            isWaiting_ = true;
//        }
    }

    setPosition(newX, newY);
}

void Pacman::setTargetForEndPoint(sf::Vector2f new_target) {
    endTarget = new_target;
}

void Pacman::setTarget(sf::Vector2f target) {
    this->target = target;
    isWaiting_ = false;
}

void Pacman::setPath(const std::vector<sf::Vector2f> &path) {
    this->path = path;
    if (!this->path.empty()) {
        target = this->path.front();
        this->path.erase(this->path.begin());
    }
}

void Pacman::checkTarget(const sf::Vector2f& newPosition) {
    if (newPosition == target) {
        if (!path.empty()) {
            target = path.front();
            path.erase(path.begin());
            isWaiting_ = false;
        } else {
            target = {-1, -1};
            isWaiting_ = true;
        }
    }
}

int Pacman::getCountCookies() {
    return countCookies;
}

void Pacman::eatCookies() {
    countCookies++;
}

bool Pacman::isWaiting() const {
    return isWaiting_;
}

const sf::Vector2f &Pacman::getEndTarget() const {
    return endTarget;
}







