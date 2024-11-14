#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>

class FileReader {
public:
    // Function to display the content of a specified file
    void displayFileContent(const std::string& filename) const;

    // Function to prompt the user for a filename and then display its content
    void promptAndDisplayFile() const;
};

#endif
