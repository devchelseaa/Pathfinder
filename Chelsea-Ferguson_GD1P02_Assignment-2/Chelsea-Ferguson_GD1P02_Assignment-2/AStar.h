#pragma once
#ifndef ASTAR_H
#define ASTAR_H

#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#include <stack>
#include <queue>
#include <cmath>
#include <iostream>
#include <fstream>
#include "MapParser.h"
#include "FileReader.h"

// Struct to store information about each node in the priority queue
struct NodeData {
    std::pair<int, int> position; // Position in the grid
    float gScore; // Cost from start to this node
    float fScore; // Total cost (g + h) from start through this node to the goal

    // Constructor
    NodeData(const std::pair<int, int>& pos, float g, float f)
        : position(pos), gScore(g), fScore(f) {}
};

// Comparison operator for the priority queue to order by lowest fScore
struct CompareF {
    bool operator()(const NodeData& a, const NodeData& b) const {
        return a.fScore > b.fScore;
    }
};

class AStar {
public:
    // Finds the optimal path using A* and optionally saves it to a map file
    void findPath(
        const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
        const std::pair<int, int>& start,
        const std::pair<int, int>& exit,
        bool saveToFile = false);

private:
    // Helper functions for distance calculations
    float manhattanDist(const std::pair<int, int>& a, const std::pair<int, int>& b) const;
    float euclideanDist(const std::pair<int, int>& a, const std::pair<int, int>& b) const;

    // Reconstructs and displays the path from start to exit
    void reconstructPath(
        const std::unordered_map<std::pair<int, int>, std::pair<int, int>, pair_hash>& cameFrom,
        const std::pair<int, int>& start,
        const std::pair<int, int>& exit,
        bool saveToFile,
        const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph) const;

    // Saves the reconstructed path to a file, marking the path with 'p'
    void savePathToFile(
        const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
        const std::vector<std::pair<int, int>>& path,
        const std::string& filename = "OptimalPath.txt") const;
};

#endif // ASTAR_H


