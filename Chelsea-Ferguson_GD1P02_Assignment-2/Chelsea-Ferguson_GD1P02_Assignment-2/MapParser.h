#ifndef MAPPARSER_H
#define MAPPARSER_H

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <fstream>
#include <iostream>
#include <unordered_map>


// Node struct representing each cell
struct Node {
    int x, y;
    char type;
};

// GraphNode struct representing each graph node, with neighbours
struct GraphNode {
    Node node; // Store the original node
    std::vector<std::pair<GraphNode*, float>> neighbours; // Pair of neighbour node and weight
};

// Hash function for std::pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

class MapParser {
public:
    // Parse the map and build the graph representation
    std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash> buildGraph(const std::string& filepath);

    // Print the graph structure for debugging
    void printGraph(const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph) const;

    std::pair<std::pair<int, int>, std::pair<int, int>> findStartAndExit(const std::vector<std::vector<Node>>& grid);

    std::vector<std::vector<Node>> parseMap(const std::string& filepath);
    void verifyBidirectionalConnections(const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph) const;

private:
    std::pair<int, int> startPos;
    std::pair<int, int> exitPos;
    // Helper functions
    void addEdgeIfValid(
        std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
        GraphNode* currentNode,
        int neighbourX,
        int neighbourY
    );
};

#endif