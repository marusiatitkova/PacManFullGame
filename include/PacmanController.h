#pragma once

#include <SFML/Graphics.hpp>
#include "Pacman.h"
#include "Ghost.h"
#include "Path_algorithms.h"

class PacmanController {
public:
    static sf::Vector2f GetDecision(const Pacman& pacman, const std::deque<Ghost>& ghosts, const Path_algorithms& pathAlgorithms);
};


