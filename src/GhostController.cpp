#include "../include/GhostController.h"
static std::mt19937 gen;

sf::Vector2f GhostController::GetDecision(const Pacman &pacman, const Ghost &ghost, const Path_algorithms& pathAlgorithms) {
    auto ghostPos = ghost.getPosition();
    auto pacmanPos = pacman.getPosition();
    auto newGhostPos = pathAlgorithms.convertCoordinatesToMaze(ghostPos);
    auto newPacmanPos = pathAlgorithms.convertCoordinatesToMaze(pacmanPos);

    std::uniform_int_distribution<> uid(0, 50);
    if (uid(gen) < 40) {

//        auto res = pathAlgorithms.a_star(newGhostPos, newPacmanPos);
//        return res[0];
        return pathAlgorithms.getBestMove(newGhostPos, newPacmanPos);
    } else { // make random move
        auto neighbours = pathAlgorithms.getNeighbours(newGhostPos);
        int rnd = rand() % (neighbours.size());
        return neighbours[rnd];
    }
}

