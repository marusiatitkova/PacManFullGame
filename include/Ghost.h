#pragma once

#include <SFML/Graphics.hpp>

class Ghost : public sf::RectangleShape {

public:
    Ghost(const std::string& pathToTexture, const sf::Vector2f& size, const sf::Vector2f& coord, float speed = 50.0f);

    Ghost(const Ghost& other) = delete;

    Ghost& operator=(const Ghost& other) = delete;

    void setTarget(sf::Vector2f target);

    virtual void update(float elapsedTime);

    bool isWaiting() const;

    void stopMoving();

private:
    float kSpeed;
    sf::Vector2f targetPoint = {-1, -1};
    sf::Texture texture;
    bool isWaiting_;
    void checkTarget(const sf::Vector2f& newPosition);
};

