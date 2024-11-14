#include "FileReader.h"
#include "MapParser.h"
#include "BFS.h"
#include "DFS.h"
#include "AStar.h"
#include <iostream>
#include <fstream>
#include <string>

void FileReader::displayFileContent(const std::string& filepath) const {
    std::ifstream file(filepath); // Attempt to open the specified file
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        return;
    }

    bool hasStart = false;
    bool hasExit = false;
    std::cout << "Contents of " << filepath << ":\n";

    while (true) {
        std::string line;
        if (!getline(file, line)) break;

        std::cout << line << std::endl;

        //Check if the line contains 's' and 'x'
        if (line.find('s') != std::string::npos) {
            hasStart = true;
        }
        if (line.find('x') != std::string::npos) {
            hasExit = true;
        }

    }

    file.close(); // Close the file after reading

    // Check if both 's' and 'x' were found
    if (!hasStart || !hasExit) {
        std::cerr << "Map does not have a valid entry/exit path" << std::endl;
    }
    else {
        // If map is valid, build and print the graph
        MapParser parser;

        // Step 1: Parse the map to get the grid
        std::vector<std::vector<Node>> grid = parser.parseMap(filepath);

        // Step 2: Find the start and exit points
        auto startExitPair = parser.findStartAndExit(grid);
        auto start = startExitPair.first;
        auto exit = startExitPair.second;

        // Step 3: Build the graph from the parsed map
        std::unordered_map<std::pair<int, int>, GraphNode*, pair_hash> graph = parser.buildGraph(filepath);

        // Step 4: Use BFS and DFS to collect items in the best order
        std::cout << "Breadth First Search" << "\n";
        BFS bfs;
        bfs.findItemsInOrder(graph, start, exit);

        std::cout << "Depth First Search" << "\n";
        DFS dfs;
        dfs.findItemsInOrder(graph, start, exit);

        // Step 5: Use A* to find the optimal path from start to exit
        std::cout << "A* Pathfinding:\n";
        AStar aStar;
        bool saveToFile = false;
        char saveChoice;

        std::cout << "Would you like to save the A* path to a file? (y/n): ";
        std::cin >> saveChoice;
        if (saveChoice == 'y' || saveChoice == 'Y') {
            saveToFile = true;
        }

        aStar.findPath(graph, start, exit, saveToFile);

        std::cout << "Start Position: (" << start.first << ", " << start.second << ")\n";
        std::cout << "Exit Position: (" << exit.first << ", " << exit.second << ")\n";

       

        std::cout << "Graph Representation:\n";

        //parser.printGraph(graph);  // Print the graph
        parser.verifyBidirectionalConnections(graph);

    }
}

void FileReader::promptAndDisplayFile() const {
    std::string filename;
    std::cout << "Enter the filename you want to read (e.g., 'example.txt'): ";
    std::getline(std::cin, filename); // Get filename input from the user

    // Prepend directory to ensure the program searches in that folder
    std::string filepath = "ExampleMaps/ExampleMaps/" + filename;

    displayFileContent(filepath); // Display the file content
}
