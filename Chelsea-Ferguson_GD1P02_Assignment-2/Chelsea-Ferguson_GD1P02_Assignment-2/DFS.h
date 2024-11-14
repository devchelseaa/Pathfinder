#ifndef DFS_H
#define DFS_H

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <utility>
#include <algorithm>
#include <iostream>
#include "MapParser.h"
#include "FileReader.h"

class DFS {
public:
    // Perform DFS to collect all items in the map in the order encountered
    void findItemsInOrder(
        const std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash>& graph,
        const std::pair<int, int>& start,
        const std::pair<int, int>& exit);
};

#endif // DFS_H