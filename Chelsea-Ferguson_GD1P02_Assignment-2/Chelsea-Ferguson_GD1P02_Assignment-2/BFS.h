#ifndef BFS_H
#define BFS_H

#include <unordered_map>
#include <queue>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_set>
#include "MapParser.h"
#include "FileReader.h"

class BFS {
public:
    // Function to find and print the optimal order to collect items using BFS
    void findItemsInOrder(
        const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
        const std::pair<int, int>& start,
        const std::pair<int, int>& exit);
};

#endif // BFS_H
