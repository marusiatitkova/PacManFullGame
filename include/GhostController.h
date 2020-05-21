#pragma once

#include <SFML/Graphics.hpp>
#include <random>
#include "Pacman.h"
#include "Ghost.h"
#include "Path_algorithms.h"

class GhostController {
public:
    static sf::Vector2f GetDecision(const Pacman& pacman, const Ghost& ghost, const Path_algorithms& pathAlgorithms);
};


