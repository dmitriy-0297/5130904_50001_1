#include <iostream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include <numeric>

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Polygon {
    std::vector<Point> points;

    bool operator==(const Polygon& other) const {
        if (points.size() != other.points.size()) return false;
        return std::equal(points.begin(), points.end(), other.points.begin());
    }
};

double polygonArea(const Polygon& poly) {
    if (poly.points.size() < 3) return 0.0;

    double area = 0.0;
    size_t n = poly.points.size();

    for (size_t i = 0; i < n; ++i) {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % n];
        area += (static_cast<double>(p1.x) * p2.y - static_cast<double>(p2.x) * p1.y);
    }

    return fabs(area) / 2.0;
}

bool parseStringToPolygon(const std::string& str, Polygon& poly) {
    std::istringstream iss(str);
    int vertexCount;

    if (!(iss >> vertexCount)) return false;
    if (vertexCount < 3) return false;

    poly.points.clear();

    for (int i = 0; i < vertexCount; ++i) {
        char openParen, semicolon, closeParen;
        Point p;

        if (!(iss >> openParen >> p.x >> semicolon >> p.y >> closeParen) ||
            openParen != '(' || semicolon != ';' || closeParen != ')') {
            return false;
        }

        poly.points.push_back(p);
    }

    std::string remaining;
    std::getline(iss, remaining);
    if (remaining.find_first_not_of(" \t\r") != std::string::npos) {
        return false;
    }

    return poly.points.size() == static_cast<size_t>(vertexCount);
}

std::vector<Polygon> readPolygon(const std::string& filename) {
    std::vector<Polygon> figures;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("ERROR: file " + filename + " is not open");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        Polygon poly;
        if (parseStringToPolygon(line, poly)) {
            figures.push_back(poly);
        }
    }
    return figures;
}

// Команда ECHO (дублирование фигур)
void handleECHO(std::istringstream& iss, std::vector<Polygon>& polygons) {
    std::string line;
    std::getline(iss, line);
    Polygon target;
    if (!parseStringToPolygon(line, target)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    int added = 0;
    for (size_t i = 0; i < polygons.size(); ++i) {
        if (polygons[i] == target) {
            polygons.insert(polygons.begin() + i + 1, polygons[i]);
            added++;
            i++;
        }
    }
    std::cout << added << '\n';
}

// Команда INFRAME (проверка вхождения в ограничивающий прямоугольник)
void handleINFRAME(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string line;
    std::getline(iss, line);
    Polygon target;
    if (!parseStringToPolygon(line, target)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }
    if (polygons.empty()) {
        std::cout << "<FALSE>\n";
        return;
    }

    int minX = polygons[0].points[0].x;
    int maxX = minX;
    int minY = polygons[0].points[0].y;
    int maxY = minY;

    for (const auto& poly : polygons) {
        for (const auto& p : poly.points) {
            if (p.x < minX) minX = p.x;
            if (p.x > maxX) maxX = p.x;
            if (p.y < minY) minY = p.y;
            if (p.y > maxY) maxY = p.y;
        }
    }

    bool inside = std::all_of(target.points.begin(), target.points.end(),
        [minX, maxX, minY, maxY](const Point& p) {
            return p.x >= minX && p.x <= maxX && p.y >= minY && p.y <= maxY;
        });

    std::cout << (inside ? "<TRUE>\n" : "<FALSE>\n");
}

void handleAREA(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string param;
    iss >> param;

    if (param == "EVEN") {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p) {
                return acc + ((p.points.size() % 2 == 0) ? polygonArea(p) : 0.0);
            });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    }
    else if (param == "ODD") {
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [](double acc, const Polygon& p) {
                return acc + ((p.points.size() % 2 == 1) ? polygonArea(p) : 0.0);
            });
        std::cout << std::fixed << std::setprecision(1) << sum << "\n";
    }
    else if (param == "MEAN") {
        if (polygons.empty()) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else {
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double acc, const Polygon& p) { return acc + polygonArea(p); });
            std::cout << std::fixed << std::setprecision(1) << (sum / polygons.size()) << "\n";
        }
    }
    else {
        try {
            int vertexCount = std::stoi(param);
            if (vertexCount < 3) {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [vertexCount](double acc, const Polygon& p) {
                    return acc + ((p.points.size() == static_cast<size_t>(vertexCount)) ? polygonArea(p) : 0.0);
                });
            std::cout << std::fixed << std::setprecision(1) << sum << "\n";
        }
        catch (...) {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}

void handleMAX(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string param;
    iss >> param;

    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "AREA") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return polygonArea(a) < polygonArea(b);
            });
        std::cout << std::fixed << std::setprecision(1) << polygonArea(*it) << "\n";
    }
    else if (param == "VERTEXES") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        std::cout << it->points.size() << "\n";
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void handleMIN(std::istringstream& iss, const std::vector<Polygon>& polygons) {
    std::string param;
    iss >> param;

    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    if (param == "AREA") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return polygonArea(a) < polygonArea(b);
            });
        std::cout << std::fixed << std::setprecision(1) << polygonArea(*it) << "\n";
    }
    else if (param == "VERTEXES") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
            [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
            });
        std::cout << it->points.size() << "\n";
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

void handleCOUNT(std::istringstream& iss, const std::vector<Polygon>& figures) {
    std::string param;
    iss >> param;

    if (param == "EVEN") {
        int cnt = std::count_if(figures.begin(), figures.end(),
            [](const Polygon& p) { return p.points.size() % 2 == 0; });
        std::cout << cnt << "\n";
    }
    else if (param == "ODD") {
        int cnt = std::count_if(figures.begin(), figures.end(),
            [](const Polygon& p) { return p.points.size() % 2 == 1; });
        std::cout << cnt << "\n";
    }
    else {
        try {
            int vertexCount = std::stoi(param);
            if (vertexCount < 3) {
                std::cout << "<INVALID COMMAND>\n";
                return;
            }
            int cnt = std::count_if(figures.begin(), figures.end(),
                [vertexCount](const Polygon& p) {
                    return p.points.size() == static_cast<size_t>(vertexCount);
                });
            std::cout << cnt << "\n";
        }
        catch (...) {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
}

void processCommand(const std::string& line, std::vector<Polygon>& polygons) {
    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "AREA") {
        handleAREA(iss, polygons);
    }
    else if (cmd == "MAX") {
        handleMAX(iss, polygons);
    }
    else if (cmd == "MIN") {
        handleMIN(iss, polygons);
    }
    else if (cmd == "COUNT") {
        handleCOUNT(iss, polygons);
    }
    else if (cmd == "ECHO") {
        handleECHO(iss, polygons);
    }
    else if (cmd == "INFRAME") {
        handleINFRAME(iss, polygons);
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            throw std::runtime_error("Usage: " + std::string(argv[0]) + " <filename>");
        }

        std::string filename = argv[1];
        std::vector<Polygon> polygons = readPolygon(filename);

        std::string line;
        while (std::getline(std::cin, line)) {
            if (line.empty()) continue;
            processCommand(line, polygons);
        }

    }
    catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
