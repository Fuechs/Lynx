#include <iostream>
#include <fstream>
#include <sstream>
#include "io.h"

std::string readFile(const std::string &filePath) {
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "could not open file '" << filePath << "'\n";
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}