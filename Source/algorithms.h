#pragma once
#include "globals.h"
#include <tuple>
#include <queue>

class Map;

typedef std::tuple<int, int, int, int> Action;

class AlgorithmResult {
public:
    Action vis[MAP_SIZE][MAP_SIZE];
    std::queue<Action> order;
};

class SearchAlgorithm {
public:
    Map* map;
    SearchAlgorithm(Map* map);
    virtual AlgorithmResult* exec() { throw - 1; }
};

class BFS : public SearchAlgorithm {
public:
    BFS(Map* map): SearchAlgorithm(map) {}
    AlgorithmResult* exec() override;
};

class DFS : public SearchAlgorithm {
public:
    DFS(Map* map) : SearchAlgorithm(map) {}
    AlgorithmResult* exec() override;
};

class AStar : public SearchAlgorithm {
public:
    AStar(Map* map) : SearchAlgorithm(map) {}
    AlgorithmResult* exec() override;
};
