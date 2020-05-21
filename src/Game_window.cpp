#include "../include/Game_window.h"

void GameWindow::handleEvents() {
    sf::Event event;
    while (pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            close();
        } else if (event.type == sf::Event::MouseButtonReleased) {
            std::cout << "mouse pressed" << std::endl;
            sf::Vector2f targetPosition(event.touch.x, event.touch.y);
            pacman->setTargetForEndPoint(targetPosition);
            pacman->setPath(pathAlgorithms.getPath(pacman->getPosition(), targetPosition));
            drawTarget = true;
        }
    }
}

void GameWindow::render() {
    clear();
    drawMaze();
    draw(*pacman);
    drawGhosts();
    drawCookies();
    if (drawTarget) {
        draw(pacman->createTarget(maze->getBlockSize()/3));
    }
    display();
}

void GameWindow::drawMaze() {
    for (unsigned int x = 0; x < maze->getWidth(); x++) {
        for (unsigned int y = 0; y < maze->getHeight(); y++) {
            draw(maze->getCells()[x][y]);
        }
    }
}

void GameWindow::drawCookies() {
    for (const auto& cookie : maze->getCookies()) {
        draw(cookie);
    }
}

void GameWindow::drawGhosts() {
    for (const Ghost& ghost : *ghosts_) {
        draw(ghost);
    }
}






