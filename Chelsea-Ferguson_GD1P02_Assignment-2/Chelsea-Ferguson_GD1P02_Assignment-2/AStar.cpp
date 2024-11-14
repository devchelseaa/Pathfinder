#include "AStar.h"

// Manhattan distance heuristic for A*
float AStar::manhattanDist(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
    return std::abs(a.first - b.first) + std::abs(a.second - b.second);
}

// Euclidean distance for path cost calculation
float AStar::euclideanDist(const std::pair<int, int>& a, const std::pair<int, int>& b) const {
    return std::sqrt(std::pow(a.first - b.first, 2) + std::pow(a.second - b.second, 2));
}

void AStar::findPath(
    const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
    const std::pair<int, int>& start,
    const std::pair<int, int>& exit,
    bool saveToFile) {

    std::priority_queue<NodeData, std::vector<NodeData>, CompareF> openSet;
    std::unordered_map<std::pair<int, int>, float, pair_hash> gScore;
    std::unordered_map<std::pair<int, int>, float, pair_hash> fScore;
    std::unordered_map<std::pair<int, int>, std::pair<int, int>, pair_hash> cameFrom;

    // Initialise start node
    gScore[start] = 0;
    fScore[start] = manhattanDist(start, exit);
    openSet.emplace(start, 0.0f, fScore[start]);

    std::cout << "Initial gScore values:" << std::endl;
    for (auto it = gScore.begin(); it != gScore.end(); ++it) {
        std::cout << "Node: (" << it->first.first << ", " << it->first.second << ") with gScore: " << it->second << std::endl;
    }

    while (!openSet.empty()) {
        NodeData current = openSet.top();
        openSet.pop();
        std::cout << "Current Node: (" << current.position.first << ", " << current.position.second << ")" << std::endl;

        // Check if reached exit
        if (current.position == exit) {
            reconstructPath(cameFrom, start, exit, saveToFile, graph);
            return;
        }

        // Get current node from graph
        auto it = graph.find(current.position);
        if (it == graph.end()) continue;
        GraphNode* currentNode = it->second;

        for (const auto& neighborPair : currentNode->neighbours) {
            GraphNode* neighborNode = neighborPair.first;
            std::pair<int, int> neighborPos = { neighborNode->node.x, neighborNode->node.y };

            // Skip walls
            if (neighborNode->node.type == '#') continue;

            float tentativeGScore = gScore[current.position] + 1;

            // Only proceed if the tentative score is better
            if (gScore.find(neighborPos) == gScore.end() || tentativeGScore < gScore[neighborPos]) {
                cameFrom[neighborPos] = current.position;
                gScore[neighborPos] = tentativeGScore;
                fScore[neighborPos] = tentativeGScore + manhattanDist(neighborPos, exit);
                openSet.emplace(neighborPos, tentativeGScore, fScore[neighborPos]);
            }
        }
    }

    std::cout << "No path found from start to exit." << std::endl;
}


// Reconstructs and displays or saves the path from start to exit
void AStar::reconstructPath(
    const std::unordered_map<std::pair<int, int>, std::pair<int, int>, pair_hash>& cameFrom,
    const std::pair<int, int>& start,
    const std::pair<int, int>& exit,
    bool saveToFile,
    const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph) const {

    std::vector<std::pair<int, int>> path;
    std::pair<int, int> current = exit;

    // Reconstruct the path by backtracking
    while (current != start) {
        path.push_back(current);
        current = cameFrom.at(current);
    }
    path.push_back(start);

    // Optionally save to file
    if (saveToFile) {
        savePathToFile(graph, path);
    }

    // Display the path
    std::cout << "Path from start to exit: ";
    for (auto it = path.rbegin(); it != path.rend(); ++it) {
        std::cout << "(" << it->first << ", " << it->second << ") ";
    }
    std::cout << std::endl;
}

// Saves the path to a file, marking the path with 'p'
void AStar::savePathToFile(
    const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
    const std::vector<std::pair<int, int>>& path,
    const std::string& filename) const {

    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error opening file to save path." << std::endl;
        return;
    }

    for (const auto& pos : path) {
        auto node = graph.find(pos);
        if (node != graph.end() && node->second->node.type != 'S' && node->second->node.type != 'E') {
            node->second->node.type = 'p';
        }
    }

    for (const auto& kvp : graph) {
        const auto& pos = kvp.first;
        const auto& node = kvp.second;
        outFile << pos.first << " " << pos.second << " " << node->node.type << std::endl;
    }

    outFile.close();
}
