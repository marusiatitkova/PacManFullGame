#include "../include/PacmanController.h"

constexpr size_t MAX_DEPTH = 6;

void rec(const sf::Vector2f& pacmanPos, const std::deque<Ghost>& ghosts, const Path_algorithms& pathAlgorithms, std::vector<std::vector<bool>>& cookies,
         double& bestScore, sf::Vector2f& bestPoint, double curScore, std::vector<sf::Vector2f>& curPath, int depth = 0) {
    if (curScore + MAX_DEPTH - depth <= bestScore) {
        return;
    }
    if (depth == MAX_DEPTH) {
        if (curScore > bestScore) {
            bestScore = curScore;
            bestPoint = curPath[0];
        }
        return;
    }

    auto neighbours = pathAlgorithms.getNeighbours(pacmanPos);

    for (const sf::Vector2f& neighbour : neighbours) {
        if (curPath.size() > 1 && neighbours.size() > 1 && curPath[curPath.size() - 2] == neighbour &&
            (static_cast<int>(neighbour.x) % 2 == 0 || static_cast<int>(neighbour.y) % 2 == 0)) {
//            continue;
        }
        curPath.push_back(neighbour);
        bool wasCookie = cookies[neighbour.x][neighbour.y];
        cookies[neighbour.x][neighbour.y] = false;
        double nextScore = curScore;
        nextScore += wasCookie * pow(0.6, depth);
        nextScore -= pathAlgorithms.getMaze()->visited[neighbour.x][neighbour.y] / 50.0f;

        for (const Ghost& ghost : ghosts) {
            sf::Vector2f ghostPos = pathAlgorithms.convertCoordinatesToMaze(ghost.getPosition());
            int dist = pathAlgorithms.getDistance(neighbour, ghostPos);
            //int dist = pathAlgorithms.a_star(neighbour, ghostPos).size();
            if (dist <= depth * 1.5) {
                if (dist <= 1) {
                    nextScore -= 1000.0f;
                }
                else {
                    nextScore -= 500.0f / dist;
                }
            }
//            if (dist <= 1) {
//                nextScore -= 1000.0f;
//            }
        }

        rec(neighbour, ghosts, pathAlgorithms, cookies, bestScore, bestPoint, nextScore, curPath, depth + 1);
        cookies[neighbour.x][neighbour.y] = wasCookie;
        curPath.pop_back();
    }

}


sf::Vector2f
PacmanController::GetDecision(const Pacman &pacman, const std::deque<Ghost> &ghosts, const Path_algorithms& pathAlgorithms) {
    size_t width = pathAlgorithms.getWidth();
    size_t height = pathAlgorithms.getHeight();
    std::vector<std::vector<bool>> cookies(width, std::vector<bool>(height, false));
    int i = 0;
    for (const auto& cookie : pathAlgorithms.getMaze()->getCookies()) {
        CellType cookieType = pathAlgorithms.getMaze()->getCookieType()[i];
        i++;
        float radius = 3.f;
        if (cookieType == CellType::FRUIT) {
            radius = 12.f;
        }
        sf::Vector2f cookiePos = {sf::Vector2f(cookie.getPosition().x + radius, cookie.getPosition().y + radius) - sf::Vector2f(0.5f * 25.f, 0.5f * 25.f)};
        sf::Vector2f p = pathAlgorithms.convertCoordinatesToMaze(cookiePos);
        cookies[p.x][p.y] = true;
    }

    sf::Vector2f pacManPos = pathAlgorithms.convertCoordinatesToMaze(pacman.getPosition());
    auto neighbours = pathAlgorithms.getNeighbours(pacManPos);
    double bestScore = -1e18;
    sf::Vector2f bestPoint = neighbours[0];
    std::vector<sf::Vector2f> path;
    rec(pacManPos, ghosts, pathAlgorithms, cookies, bestScore, bestPoint, 0, path, 0);

    return bestPoint;
}
