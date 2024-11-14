#include <iostream>
#include "FileReader.h"

void displayMenu() {
    std::cout << "1. Read and display file content\n";
    std::cout << "2. Exit\n";
    std::cout << "Enter your choice: ";
}

int main() {
    FileReader fileReader;
    int choice;

    do {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
        case 1:
            fileReader.promptAndDisplayFile();
            break;
        case 2:
            std::cout << "Exiting the program.\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
        }
    } while (choice != 2);

    return 0;
}
