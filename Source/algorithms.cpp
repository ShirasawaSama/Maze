#include "algorithms.h"
#include "Map.h"
#include <queue>

SearchAlgorithm::SearchAlgorithm(Map* map) {
    this->map = map;
    int n = map->getMapHeight(), m = map->getMapWidth();
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (map->matrix[i][j] == 2) {
                map->startY = i;
                map->startX = j;
                return;
            }
        }
    }
}

AlgorithmResult* BFS::exec() {
    std::queue<Action> q;
    int n = map->getMapHeight(), m = map->getMapWidth();
    q.push(std::make_tuple(map->startX, map->startY, 0, 0));
    bool vis[MAP_SIZE][MAP_SIZE];
    memset(vis, 0, sizeof(vis));
    auto result = new AlgorithmResult();
    vis[map->startY][map->startY] = true;
    while (!q.empty()) {
        auto cur = q.front();
        int x, y, preX, preY;
        std::tie(x, y, preX, preY) = cur;
        result->order.push(cur);
        result->vis[y][x] = cur;
        if (map->matrix[y][x] == 3) break;
        q.pop();
        if (x > 0 && !vis[y][x - 1] && map->matrix[y][x - 1] != 1) {
            q.push(std::make_tuple(x - 1, y, x, y));
            vis[y][x - 1] = true;
        }
        if (x < m - 1 && !vis[y][x + 1] && map->matrix[y][x + 1] != 1) {
            q.push(std::make_tuple(x + 1, y, x, y));
            vis[y][x + 1] = true;
        }
        if (y > 0 && !vis[y - 1][x] && map->matrix[y - 1][x] != 1) {
            q.push(std::make_tuple(x, y - 1, x, y));
            vis[y - 1][x] = true;
        }
        if (y < n - 1 && !vis[y + 1][x] && map->matrix[y + 1][x] != 1) {
            q.push(std::make_tuple(x, y + 1, x, y));
            vis[y + 1][x] = true;
        }
    }
    return result;
}

AlgorithmResult* DFS::exec() {
    // TODO: DFS
    return nullptr;
}

AlgorithmResult* AStar::exec() {
    // TODO: Astar
    return nullptr;
}
