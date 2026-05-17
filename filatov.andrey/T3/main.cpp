#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "polygon.h"
#include "commands.h"

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::ifstream file(filename);
    if (!file.is_open()) return polygons;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        Polygon p = parsePolygon(line);
        if (!p.points.empty())
            polygons.push_back(p);
    }
    return polygons;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::vector<Polygon> polygons = readPolygons(argv[1]);
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        if (cmd == "AREA") {
            std::string arg;
            iss >> arg;
            if (arg.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            processArea(polygons, arg);
        }
        else if (cmd == "MAX") {
            std::string arg;
            iss >> arg;
            if (arg.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            processMax(polygons, arg);
        }
        else if (cmd == "MIN") {
            std::string arg;
            iss >> arg;
            if (arg.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            processMin(polygons, arg);
        }
        else if (cmd == "COUNT") {
            std::string arg;
            iss >> arg;
            if (arg.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            processCount(polygons, arg);
        }
        else if (cmd == "RMECHO") {
            std::string rest;
            std::getline(iss, rest);
            if (rest.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            Polygon target = parsePolygon(rest);
            if (target.points.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            processRmecho(polygons, target);
        }
        else if (cmd == "INTERSECTIONS") {
            std::string rest;
            std::getline(iss, rest);
            if (rest.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            Polygon target = parsePolygon(rest);
            if (target.points.empty()) { std::cout << "<INVALID COMMAND>" << std::endl; continue; }
            processIntersections(polygons, target);
        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }
    return 0;
}
