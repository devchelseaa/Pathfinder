#include "MapParser.h"
#include <algorithm>
#include "AStar.h"

std::vector<std::vector<Node>> MapParser::parseMap(const std::string& filepath) {
    std::ifstream file(filepath);
    std::vector<std::vector<Node>> grid;
    std::string line;
    int y = 0;

    while (std::getline(file, line)) {
        std::vector<Node> row;
        for (int x = 0; x < line.size(); ++x) {
            row.push_back({ x, y, line[x] });
        }
        grid.push_back(row);
        ++y;
    }
    return grid;
}

// Builds the graph by creating nodes and adding edges for non-wall cells
std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash> MapParser::buildGraph(const std::string& filepath) {
    std::vector<std::vector<Node>> grid = parseMap(filepath);
    std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash> graph;

    // Create GraphNode objects for each cell that is not a wall
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x].type != 'w') {
                graph[{x, y}] = new GraphNode{ grid[y][x] };
            }
        }
    }

    // Connect the adjacent nodes
    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x].type != 'w') {
                GraphNode* currentNode = graph[{x, y}];
                addEdgeIfValid(graph, currentNode, x + 1, y); // Right
                addEdgeIfValid(graph, currentNode, x - 1, y); // Left
                addEdgeIfValid(graph, currentNode, x, y + 1); // Down
                addEdgeIfValid(graph, currentNode, x, y - 1); // Up
            }
        }
    }

    // Verify bidirectional connections after building the graph
    verifyBidirectionalConnections(graph);

    return graph;
}

void MapParser::verifyBidirectionalConnections(const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph) const {
    // Iterate over each GraphNode in the graph
    for (const auto& nodePair : graph) {
        const auto& position = nodePair.first; // This is the key (x, y)
        GraphNode* graphNode = nodePair.second; // This is the GraphNode* 

        // For each neighbor of the current graphNode
        for (const auto& neighbourPair : graphNode->neighbours) {
            GraphNode* neighbour = neighbourPair.first; // Access the neighbour GraphNode*
            bool bidirectional = false;

            // Check if the neighbour has a back connection to the current graphNode
            for (const auto& backNeighbourPair : neighbour->neighbours) {
                GraphNode* backNeighbour = backNeighbourPair.first;
                if (backNeighbour == graphNode) { // Check if the back neighbor is the original node
                    bidirectional = true;
                    break;
                }
            }

            // If no bidirectional connection is found, print a warning
            if (!bidirectional) {
                std::cerr << "Warning: Node (" << position.first << ", " << position.second
                    << ") has a neighbor (" << neighbour->node.x << ", " << neighbour->node.y
                    << ") without bidirectional connection.\n";
            }
        }
    }
}



// Adds an edge between the current node and a valid neighbouring node
void MapParser::addEdgeIfValid(
    std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
    GraphNode* currentNode,
    int neighbourX,
    int neighbourY
) {
    auto neighbourPos = std::make_pair(neighbourX, neighbourY);
    if (graph.find(neighbourPos) != graph.end()) {
        GraphNode* neighbourNode = graph[neighbourPos];

        // Check if the neighbour is already connected to avoid duplicates
        if (std::none_of(currentNode->neighbours.begin(), currentNode->neighbours.end(),
            [neighbourNode](const auto& pair) { return pair.first == neighbourNode; })) {

            // Calculate Euclidean distance as the weight
            float weight = std::sqrt(std::pow(neighbourX - currentNode->node.x, 2) +
                std::pow(neighbourY - currentNode->node.y, 2));

            // Add neighbour with weight
            currentNode->neighbours.emplace_back(neighbourNode, weight);
        }
    }
}

// Prints the graph representation
void MapParser::printGraph(const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph) const {
    for (const auto& item : graph) {
        auto pos = item.first;
        auto* node = item.second;
        std::cout << "Node (" << pos.first << ", " << pos.second << ") - Type: " << node->node.type << " - Neighbours: ";
        for (const auto& neighbour : node->neighbours) {
            auto* neighbourNode = neighbour.first;
            float weight = neighbour.second;
            std::cout << "(" << neighbourNode->node.x << ", " << neighbourNode->node.y << ", Weight: " << weight << ") ";
        }
        std::cout << '\n';
    }
}

std::pair<std::pair<int, int>, std::pair<int, int>> MapParser::findStartAndExit(const std::vector<std::vector<Node>>& grid) {
    std::pair<int, int> start(-1, -1);
    std::pair<int, int> exit(-1, -1);

    for (int y = 0; y < grid.size(); ++y) {
        for (int x = 0; x < grid[y].size(); ++x) {
            if (grid[y][x].type == 's') {
                start = { x, y };
            }
            else if (grid[y][x].type == 'x') {
                exit = { x, y };
            }
        }
    }

    return { start, exit };
}



