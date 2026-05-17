#include "commands.h"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <vector>
#include <string>

void processArea(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (arg == "MEAN") {
        if (polygons.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double sum, const Polygon& p) { return sum + polygonArea(p); });
        std::cout << std::fixed << std::setprecision(1) << total / polygons.size() << std::endl;
        return;
    }

    if (arg == "EVEN" || arg == "ODD") {
        if (polygons.empty()) {
            std::cout << "0.0" << std::endl;
            return;
        }
        bool isEven = (arg == "EVEN");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [isEven](double s, const Polygon& p) {
                return s + (((p.points.size() % 2 == 0) == isEven) ? polygonArea(p) : 0.0);
            });
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
        return;
    }

    // аргумент – число вершин
    int vertexCount;
    try {
        vertexCount = std::stoi(arg);
    } catch (...) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (vertexCount < 3) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (polygons.empty()) {
        std::cout << "0.0" << std::endl;
        return;
    }
    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [vertexCount](double s, const Polygon& p) {
            return s + (p.points.size() == static_cast<size_t>(vertexCount) ? polygonArea(p) : 0.0);
        });
    std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
}

void processMax(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (arg == "AREA") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return polygonArea(a) < polygonArea(b); });
        std::cout << std::fixed << std::setprecision(1) << polygonArea(*it) << std::endl;
    } else if (arg == "VERTEXES") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
        std::cout << it->points.size() << std::endl;
    } else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void processMin(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (arg == "AREA") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return polygonArea(a) < polygonArea(b); });
        std::cout << std::fixed << std::setprecision(1) << polygonArea(*it) << std::endl;
    } else if (arg == "VERTEXES") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) { return a.points.size() < b.points.size(); });
        std::cout << it->points.size() << std::endl;
    } else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void processCount(const std::vector<Polygon>& polygons, const std::string& arg) {
    if (arg == "EVEN" || arg == "ODD") {
        bool isEven = (arg == "EVEN");
        int cnt = std::count_if(polygons.begin(), polygons.end(),
            [isEven](const Polygon& p) { return (p.points.size() % 2 == 0) == isEven; });
        std::cout << cnt << std::endl;
        return;
    }
    int vertexCount;
    try {
        vertexCount = std::stoi(arg);
    } catch (...) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (vertexCount < 3) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    int cnt = std::count_if(polygons.begin(), polygons.end(),
        [vertexCount](const Polygon& p) { return p.points.size() == static_cast<size_t>(vertexCount); });
    std::cout << cnt << std::endl;
}

void processRmecho(std::vector<Polygon>& polygons, const Polygon& target) {
    int removed = 0;
    bool lastWasTarget = false;
    auto newEnd = std::remove_if(polygons.begin(), polygons.end(),
        [&](const Polygon& p) mutable {
            if (p == target) {
                if (lastWasTarget) {
                    ++removed;
                    return true;
                } else {
                    lastWasTarget = true;
                    return false;
                }
            } else {
                lastWasTarget = false;
                return false;
            }
        });
    polygons.erase(newEnd, polygons.end());
    std::cout << removed << std::endl;
}

void processIntersections(const std::vector<Polygon>& polygons, const Polygon& target) {
    int cnt = std::count_if(polygons.begin(), polygons.end(),
        [&](const Polygon& p) { return polygonsIntersect(p, target); });
    std::cout << cnt << std::endl;
}
