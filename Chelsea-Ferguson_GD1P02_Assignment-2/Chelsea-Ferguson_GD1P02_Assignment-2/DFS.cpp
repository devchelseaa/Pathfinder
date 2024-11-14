#include "DFS.h"

void DFS::findItemsInOrder(
    const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
    const std::pair<int, int>& start,
    const std::pair<int, int>& exit) {

    std::stack<std::pair<int, int>> stack;
    std::unordered_set<std::pair<int, int>, pair_hash> visited;
    std::vector<char> collectedItems;

    // Start the DFS traversal
    stack.push(start);
    visited.insert(start);

    while (!stack.empty()) {
        auto currentPos = stack.top();
        stack.pop();

        // Access the current node from the graph
        auto it = graph.find(currentPos);
        if (it == graph.end()) continue; // Skip if the node isn't found in the graph
        GraphNode* currentNode = it->second;

        // Check if the current node contains an item (between 'a' and 'j')
        if (currentNode->node.type >= 'a' && currentNode->node.type <= 'j') {
            // Only add the item if it hasn’t been collected yet
            if (std::find(collectedItems.begin(), collectedItems.end(), currentNode->node.type) == collectedItems.end()) {
                collectedItems.push_back(currentNode->node.type);
            }
        }

        // Visit all neighboring nodes in the original order to maintain DFS logic
        for (auto& neighbour : currentNode->neighbours) {
            GraphNode* neighbourNode = neighbour.first;
            std::pair<int, int> neighbourPos = { neighbourNode->node.x, neighbourNode->node.y };

            if (visited.find(neighbourPos) == visited.end()) {
                stack.push(neighbourPos);
                visited.insert(neighbourPos);
            }
        }
    }

    // Print all collected items in the order they were encountered
    std::cout << "Collected Items in Order: ";
    for (char item : collectedItems) {
        std::cout << item << " ";
    }
    std::cout << std::endl;
}
