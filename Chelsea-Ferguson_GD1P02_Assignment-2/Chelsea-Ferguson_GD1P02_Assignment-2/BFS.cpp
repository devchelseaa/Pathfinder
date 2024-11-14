#include "BFS.h"

void BFS::findItemsInOrder(
    const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
    const std::pair<int, int>& start,
    const std::pair<int, int>& exit) {

    std::queue<std::pair<int, int>> queue;
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::vector<char> collectedItems; // To keep track of items in the order they are collected

    queue.push(start);
    visited.insert(start);

    while (!queue.empty()) {
        auto currentPos = queue.front();
        queue.pop();

        // Access the current node from the graph
        auto it = graph.find(currentPos);
        if (it == graph.end()) continue; // If node not found, skip it
        GraphNode* currentNode = it->second;

        // Check if the current node contains an item (between 'a' and 'j')
        if (currentNode->node.type >= 'a' && currentNode->node.type <= 'j') {
            // Add the item if it's not already collected
            if (std::find(collectedItems.begin(), collectedItems.end(), currentNode->node.type) == collectedItems.end()) {
                collectedItems.push_back(currentNode->node.type);
            }
        }

        // Stop if all items ('a' to 'j') have been collected
        if (collectedItems.size() == 10) break;

        // Visit all neighboring nodes
        for (const auto& neighbour : currentNode->neighbours) {
            GraphNode* neighbourNode = neighbour.first;

            std::pair<int, int> neighbourPos = { neighbourNode->node.x, neighbourNode->node.y };
            if (visited.find(neighbourPos) == visited.end()) {
                queue.push(neighbourPos);
                visited.insert(neighbourPos);
            }
        }
    }

    // Print the collected items in the order they were found
    std::cout << "Collected Items in Order: ";
    for (char item : collectedItems) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}
