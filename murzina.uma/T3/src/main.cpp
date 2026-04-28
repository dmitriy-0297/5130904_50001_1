#include "Geometry.hpp"
#include "Commands.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Filename parameter is required" << std::endl;
        return 1;
    }
    
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
        return 1;
    }
    
    std::vector<geometry::Polygon> polygons;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        geometry::Polygon poly;
        
        if (iss >> poly) {
            polygons.push_back(poly);
        }
    }
    
    file.close();
    
    CommandProcessor processor(std::move(polygons));
    std::string command;
    
    while (std::getline(std::cin, command)) {
        if (!command.empty()) {
            processor.processCommand(command);
        }
    }
    
    return 0;
}
