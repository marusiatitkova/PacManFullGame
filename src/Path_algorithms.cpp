#include <iomanip>
#include "../include/Path_algorithms.h"
constexpr int INF = 9999999;

struct Statistics {
    explicit Statistics(const std::string& name) {
        this->name = name;
        start = std::chrono::steady_clock::now();
    }

    void setDuration() {
        auto end = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    }

    void increaseIterations() {
        iterations++;
    }

    void setPathLength (const int& len) {
        pathLength = len;
    }

    void addMemoryUsage(int memory) {
        curMemoryUsage += memory;
        memoryUsage = std::max(memoryUsage, curMemoryUsage);
    }

    void freeMemoryUsage(int memory) {
        curMemoryUsage -= memory;
    }

    std::string name;
    int pathLength{0};
    int iterations{0};
    std::chrono::steady_clock::time_point start;
    double duration{0};
    int memoryUsage{0};
    int curMemoryUsage{0};
};

std::ostream& operator << (std::ostream& stream, const Statistics& statistics) {
    stream << "Statistics for " << statistics.name << "\n";
    stream << "Execution Time: " << statistics.duration << "\n";
    stream << "Path Length: " << statistics.pathLength << "\n";
    stream << "Number of Iterations: " << statistics.iterations << "\n";
    stream << "Memory Usage: " << statistics.memoryUsage << "\n";
    return stream;
}


Path_algorithms::Path_algorithms(const size_t& _width, const size_t& _height, Maze* _maze)
                            : width(_width), height(_height), maze(_maze) {
    floyd_Warshall();
}

std::ostream& operator << (std::ostream& stream, const sf::Vector2f& vector2F) {
    stream << vector2F.x << " " << vector2F.y;
    return stream;
}

std::vector<sf::Vector2f> Path_algorithms::dfs(const sf::Vector2f &start, const sf::Vector2f &end) {
    Statistics dfsStat("DFS");
    std::vector <sf::Vector2f> path;
    std::map<sf::Vector2f, bool> pathPoint_to_isVisited;
    path.push_back(start);
    pathPoint_to_isVisited[start] = true;
    dfs_recursion(start, end, pathPoint_to_isVisited, path, dfsStat);
    dfsStat.setDuration();
    dfsStat.setPathLength(path.size());
    std::cout << dfsStat << std::endl;
    return path;
}

bool Path_algorithms::dfs_recursion(const sf::Vector2f& from, const sf::Vector2f& end, std::map<sf::Vector2f,
        bool>& pathPoint_to_isVisited,  std::vector <sf::Vector2f>& path, Statistics& dfsStat) {
    dfsStat.increaseIterations();
    dfsStat.addMemoryUsage(sizeof(pathPoint_to_isVisited.begin()));
    if (from == end) {
        dfsStat.freeMemoryUsage(sizeof(pathPoint_to_isVisited.begin()));
        return true;
    }

    for (const auto &n : getNeighbours(from)) {
        if (pathPoint_to_isVisited.count(n) == 0) {
            pathPoint_to_isVisited[n] = false;
        }

        if (!pathPoint_to_isVisited[n]) {
            pathPoint_to_isVisited[n] = true;
            dfsStat.addMemoryUsage(sizeof(pathPoint_to_isVisited.begin()));
            path.push_back(n);
            bool found = dfs_recursion(n, end, pathPoint_to_isVisited, path, dfsStat);
            if (found) {
                return true;
            } else
                path.pop_back();
        }
    }
    return false;
}

std::vector<sf::Vector2f> Path_algorithms::bfs(const sf::Vector2f &start, const sf::Vector2f &end) {
    Statistics bfsStat("BFS");
    std::queue<sf::Vector2f> q;
    std::vector<std::vector<bool>> visited(width);
    std::vector<std::vector<sf::Vector2f>> parent(width);
    for (size_t x = 0; x < width; x++) {
        visited[x].resize(height);
        parent[x].resize(height);
        for (size_t y = 0; y < height; y++) {
            visited[x][y] = false;
        }
    }
    q.push(start);
    visited[start.x][start.y] = true;
    parent[start.x][start.y] = start;
    bfsStat.addMemoryUsage(sizeof(q.front()));
    while (!q.empty()) {
        bfsStat.increaseIterations();
        auto cur = q.front();
        bfsStat.freeMemoryUsage(sizeof(q.front()));
        q.pop();
        for (const auto& n : getNeighbours(cur)) {
            if (!visited[n.x][n.y]) {
                visited[n.x][n.y] = true;
                parent[n.x][n.y] = cur;
                if (n == end) {
                    break;
                } else {
                    q.push(n);
                    bfsStat.addMemoryUsage(sizeof(q.front()));
                }
            }
        }
    }
    std::vector<sf::Vector2f> path = getPathByParent(parent, start, end);

    bfsStat.setDuration();
    bfsStat.setPathLength(path.size());
    std::cout << bfsStat << std::endl;
    return path;
}

std::vector<sf::Vector2f> Path_algorithms::greedy(const sf::Vector2f &start, const sf::Vector2f &end) {
    Statistics greedyStat("GREEDY");
    std::queue<sf::Vector2f> q;
    std::vector<std::vector<int>> distance(width);
    std::vector<std::vector<bool>> visited(width);
    std::vector<std::vector<sf::Vector2f>> parent(width);
    for (size_t x = 0; x < width; x++) {
        distance[x].resize(height);
        visited[x].resize(height);
        parent[x].resize(height);
        for (size_t y = 0; y < height; y++) {
            distance[x][y] = INT_MAX;
            visited[x][y] = false;
        }
    }
    distance[start.x][start.y] = heuristic(start, end);
    q.push(start);
    parent[start.x][start.y] = start;
    greedyStat.addMemoryUsage(sizeof(q.front()));
    while (!q.empty()) {
        greedyStat.increaseIterations();
        auto cur = q.front();
        greedyStat.freeMemoryUsage(sizeof(q.front()));
        q.pop();
        visited[cur.x][cur.y] = true;
        if (cur == end) {
            break;
        }
        for(const auto& n : getNeighbours(cur)) {
            if (visited[n.x][n.y]) {
                continue;
            } else {
                distance[cur.x][cur.y] = heuristic(cur, end);
                parent[n.x][n.y] = cur;
                q.push(n);
                greedyStat.addMemoryUsage(sizeof(q.front()));
            }
        }
    }

    std::vector<sf::Vector2f> path = getPathByParent(parent, start, end);

    greedyStat.setDuration();
    greedyStat.setPathLength(path.size());
    std::cout << greedyStat << std::endl;
    return path;
}

int Path_algorithms::heuristic(const sf::Vector2f& from, const sf::Vector2f& to) {
    return abs(from.x - to.x) + abs(from.y - to.y);
}



std::vector<sf::Vector2f> Path_algorithms::a_star(const sf::Vector2f &start, const sf::Vector2f &end) const{
    Statistics a_starStat("A_STAR");
    if (start == end) {
        return {start};
    }
    auto comp = [](const std::pair<int, sf::Vector2f>& lhs, const std::pair<int, sf::Vector2f>& rhs)
            {if (lhs.first == rhs.first) {
                return lhs.second.x < rhs.second.x || (lhs.second.x == rhs.second.x && lhs.second.y < rhs.second.y);
            }
                return lhs.first < rhs.first;
            };
    auto pq = std::set<std::pair<int, sf::Vector2f>, decltype(comp)>(comp);
    std::vector<std::vector<int>> total_cost(width);
    std::vector<std::vector<int>> cost_from_start(width);
    std::vector<std::vector<sf::Vector2f>> parent(width);
    for (size_t x = 0; x < width; x++) {
        total_cost[x].resize(height);
        cost_from_start[x].resize(height);
        parent[x].resize(height);
        for (size_t y = 0; y < height; y++) {
            total_cost[x][y] = INT_MAX;
            parent[x][y] = {-1, -1};
        }
    }
    cost_from_start[start.x][start.y] = 0;
    total_cost[start.x][start.y] = cost_from_start[start.x][start.y] + heuristic(start, end);
    pq.insert({total_cost[start.x][start.y], start});
    parent[start.x][start.y] = start;
    a_starStat.addMemoryUsage(sizeof(pq.begin()));
    while (!pq.empty()) {
        a_starStat.increaseIterations();
        auto cur = pq.begin()->second;
        a_starStat.freeMemoryUsage(sizeof(pq.begin()));
        pq.erase(pq.begin());
        if (cur == end) {
            break;
        }
        for(const auto& n : getNeighbours(cur)) {
            cost_from_start[n.x][n.y] = cost_from_start[cur.x][cur.y] + 1;
            auto temp = cost_from_start[cur.x][cur.y] + 1 + heuristic(n, end);
            if (temp < total_cost[n.x][n.y]) {
                total_cost[n.x][n.y] = temp;
                parent[n.x][n.y] = cur;
                pq.insert({total_cost[n.x][n.y], n});
                a_starStat.addMemoryUsage(sizeof(pq.begin()));
            }
        }
    }

    std::vector<sf::Vector2f> path = getPathByParent(parent, start, end);
    a_starStat.setDuration();
    a_starStat.setPathLength(path.size());
    std::cout << a_starStat << std::endl;
    return path;
}

std::vector<sf::Vector2f> Path_algorithms::getNeighbours(const sf::Vector2f &cur) const{
    std::vector<sf::Vector2f> neighbours;
    for (const auto& [key, value] : neighboursMoves) {
        sf::Vector2f to = cur + value;
        if (canMove(to)) {
            neighbours.push_back(to);
        }
    }
    return neighbours;
}

bool Path_algorithms::canMove(const sf::Vector2f &to) const{

    return to.x >= 0 && to.y >= 0 && to.x < width && to.y < height && !maze->getCells()[to.x][to.y].isWall(); //getCellbyPosition(to * maze->getBlockSize()).isWall()
}

void Path_algorithms::showPath(const std::vector<sf::Vector2f> &path) {
    if (path.empty()) {
        std::cout << "Path is empty" << std::endl;
    }
    for (const auto& p : path) {
        std::cout << p << " -> ";
    }
    std::cout << std::endl;
}

std::vector<sf::Vector2f> Path_algorithms::getPath(const sf::Vector2f &from, const sf::Vector2f &to) {
    sf::Vector2f start = convertCoordinatesToMaze(from);
    sf::Vector2f end = convertCoordinatesToMaze(to);

    std::vector<sf::Vector2f> res_dfs = convertPathToCells(dfs(start, end));
    std::vector<sf::Vector2f> res_bfs = convertPathToCells(bfs(start, end));
    std::vector<sf::Vector2f> res_greedy = convertPathToCells(greedy(start, end));
    std::vector<sf::Vector2f> res_a_star = convertPathToCells(a_star(start, end));
    return res_a_star;
}

std::vector<sf::Vector2f> Path_algorithms::convertPathToCells(const std::vector<sf::Vector2f> &path) {
    std::vector<sf::Vector2f> convertedPath;
    for (const auto& point : path) {
        convertedPath.emplace_back(point.x * maze->getBlockSize(), point.y *maze->getBlockSize());
    }
    return convertedPath;
}

sf::Vector2f Path_algorithms::convertCoordinatesToMaze(const sf::Vector2f &cur) const{
    sf::Vector2f res = {cur.x / maze->getBlockSize(), cur.y / maze->getBlockSize()};
    res.x = static_cast<int>(res.x);
    res.y = static_cast<int>(res.y);
    return res;
}

std::vector<sf::Vector2f> Path_algorithms::getPathByParent(const std::vector<std::vector<sf::Vector2f>>& parent, const sf::Vector2f &start,
                                      const sf::Vector2f &end) {
    std::vector<sf::Vector2f> path;
    for (auto p = end; p != start; p = parent[p.x][p.y]) {
        path.push_back (p);
    }
    reverse (path.begin(), path.end());
    return path;
}

size_t Path_algorithms::getWidth() const {
    return width;
}

size_t Path_algorithms::getHeight() const {
    return height;
}

Maze *Path_algorithms::getMaze() const {
    return maze;
}

void Path_algorithms::floyd_Warshall() {
    distance.resize(width);
    for (size_t x = 0; x < width; x++) {
        distance[x].resize(height);
        for (size_t y = 0; y < height; y++) {
            distance[x][y].resize(width);
            for (size_t x_to = 0; x_to < width; x_to++) {
                distance[x][y][x_to].resize(height);
                for (size_t y_to = 0; y_to < height; y_to++) {
                    distance[x][y][x_to][y_to] = INF;
                }
            }
        }
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            sf::Vector2f pos(x, y);
            if (!canMove(pos)) {
                continue;
            }
            for (const auto& neighbour : getNeighbours(pos)) {
                distance[x][y][neighbour.x][neighbour.y] = 1;
                distance[neighbour.x][neighbour.y][x][y] = 1;
            }
            distance[x][y][x][y] = 0;
        }
    }

    for (int kx = 0; kx < width; ++kx) {
        for (int ky = 0; ky < height; ++ky) {
            for (int ix = 0; ix < width; ++ix) {
                for (int iy = 0; iy < height; ++iy) {
                    for (int jx = 0; jx < width; ++jx) {
                        for (int jy = 0; jy < height; ++jy) {
                            if (distance[ix][iy][kx][ky] < INF && distance[kx][ky][jx][jy] < INF && distance[ix][iy][jx][jy] > distance[ix][iy][kx][ky] + distance[kx][ky][jx][jy]) {
                                distance[ix][iy][jx][jy] = distance[ix][iy][kx][ky] + distance[kx][ky][jx][jy];
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "Distance constructed\n";

//    for (int x = 0; x < width; x++) {
//        for (int y = 0; y < height; y++) {
//            //sf::Vector2f position(width, height);
//            int distance = dist[12][9][x][y];
//            if (distance == INF) {
//                distance = -1;
//            }
//            std::cout << std::setw(3) << distance << ' ';
//        }
//        std::cout << '\n';
//    }
}

int Path_algorithms::getDistance(sf::Vector2f from, sf::Vector2f to) const {
    if (distance[from.x][from.y][to.x][to.y] == INF) {
        return -1;
    }
    return distance[from.x][from.y][to.x][to.y];
}

sf::Vector2f Path_algorithms::getBestMove(sf::Vector2f from, sf::Vector2f to) const {
    int dist = distance[from.x][from.y][to.x][to.y];
    for (const auto& neighbour : getNeighbours(from)) {
        int distFromNeighbour =  distance[neighbour.x][neighbour.y][to.x][to.y];
        if (dist == distFromNeighbour + 1) {
            return neighbour;
        }
    }
    return from;
}



