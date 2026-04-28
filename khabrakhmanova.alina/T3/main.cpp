#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <map>
#include <set>

struct Point {
    int x, y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        return x < other.x || (x == other.x && y < other.y);
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
        area += static_cast<double>(p1.x * p2.y - p2.x * p1.y);
    }
    return std::abs(area) / 2.0;
}

bool isRightAngle(const Point& a, const Point& b, const Point& c) {
    long long ax = b.x - a.x;
    long long ay = b.y - a.y;
    long long cx = b.x - c.x;
    long long cy = b.y - c.y;
    return (ax * cx + ay * cy) == 0;
}

bool hasRightAngle(const Polygon& poly) {
    size_t n = poly.points.size();
    if (n < 3) return false;

    for (size_t i = 0; i < n; ++i) {
        const Point& prev = poly.points[(i - 1 + n) % n];
        const Point& curr = poly.points[i];
        const Point& next = poly.points[(i + 1) % n];
        if (isRightAngle(prev, curr, next)) {
            return true;
        }
    }
    return false;
}

bool isPermutation(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) return false;

    std::multiset<Point> setA(a.points.begin(), a.points.end());
    std::multiset<Point> setB(b.points.begin(), b.points.end());
    return setA == setB;
}

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return {};
    }

    std::vector<Polygon> polygons;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        int n;
        if (!(iss >> n)) continue;

        if (n < 3) continue;

        Polygon poly;
        bool valid = true;

        for (int i = 0; i < n; ++i) {
            char open, comma, close;
            int x, y;

            if (!(iss >> open >> x >> comma >> y >> close) ||
                open != '(' || comma != ';' || close != ')') {
                valid = false;
                break;
            }
            poly.points.push_back({x, y});
        }

        if (valid && poly.points.size() == static_cast<size_t>(n)) {
            polygons.push_back(poly);
        }
    }

    return polygons;
}

Polygon parsePolygonFromString(const std::string& str) {
    std::istringstream iss(str);
    Polygon poly;
    int n;

    if (!(iss >> n)) return poly;

    if (n < 3) return poly;

    for (int i = 0; i < n; ++i) {
        char open, comma, close;
        int x, y;
        if (iss >> open >> x >> comma >> y >> close) {
            if (open == '(' && comma == ';' && close == ')') {
                poly.points.push_back({x, y});
            }
        }
    }

    if (poly.points.size() != static_cast<size_t>(n)) {
        poly.points.clear();
    }

    return poly;
}

bool hasVertexCount(const std::vector<Polygon>& polygons, int count) {
    return std::any_of(polygons.begin(), polygons.end(),
        [count](const Polygon& p) {
            return static_cast<int>(p.points.size()) == count;
        });
}

void cmdArea(const std::vector<Polygon>& polygons, const std::string& param) {
    try {
        double result = 0.0;

        if (param == "EVEN") {
            result = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double sum, const Polygon& p) {
                    return sum + ((p.points.size() % 2 == 0) ? polygonArea(p) : 0.0);
                });
        }
        else if (param == "ODD") {
            result = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double sum, const Polygon& p) {
                    return sum + ((p.points.size() % 2 != 0) ? polygonArea(p) : 0.0);
                });
        }
        else if (param == "MEAN") {
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                return;
            }
            double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double s, const Polygon& p) { return s + polygonArea(p); });
            result = sum / polygons.size();
        }
        else {
            int vertexCount = std::stoi(param);
            if (vertexCount < 3 || !hasVertexCount(polygons, vertexCount)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                return;
            }
            result = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [vertexCount](double sum, const Polygon& p) {
                    return sum + ((p.points.size() == static_cast<size_t>(vertexCount)) ? polygonArea(p) : 0.0);
                });
        }

        std::cout << std::fixed << std::setprecision(1) << result << std::endl;
    }
    catch (const std::invalid_argument&) {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void cmdMax(const std::vector<Polygon>& polygons, const std::string& param) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }

    try {
        if (param == "AREA") {
            auto maxIt = std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return polygonArea(a) < polygonArea(b);
                });
            std::cout << std::fixed << std::setprecision(1) << polygonArea(*maxIt) << std::endl;
        }
        else if (param == "VERTEXES") {
            auto maxIt = std::max_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return a.points.size() < b.points.size();
                });
            std::cout << maxIt->points.size() << std::endl;
        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }
    catch (const std::exception&) {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void cmdMin(const std::vector<Polygon>& polygons, const std::string& param) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }

    try {
        if (param == "AREA") {
            auto minIt = std::min_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return polygonArea(a) < polygonArea(b);
                });
            std::cout << std::fixed << std::setprecision(1) << polygonArea(*minIt) << std::endl;
        }
        else if (param == "VERTEXES") {
            auto minIt = std::min_element(polygons.begin(), polygons.end(),
                [](const Polygon& a, const Polygon& b) {
                    return a.points.size() < b.points.size();
                });
            std::cout << minIt->points.size() << std::endl;
        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }
    catch (const std::exception&) {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void cmdCount(const std::vector<Polygon>& polygons, const std::string& param) {
    try {
        size_t result = 0;

        if (param == "EVEN") {
            result = std::count_if(polygons.begin(), polygons.end(),
                [](const Polygon& p) { return p.points.size() % 2 == 0; });
        }
        else if (param == "ODD") {
            result = std::count_if(polygons.begin(), polygons.end(),
                [](const Polygon& p) { return p.points.size() % 2 != 0; });
        }
        else {
            int vertexCount = std::stoi(param);
            if (vertexCount < 3 || !hasVertexCount(polygons, vertexCount)) {
                std::cout << "<INVALID COMMAND>" << std::endl;
                return;
            }
            result = std::count_if(polygons.begin(), polygons.end(),
                [vertexCount](const Polygon& p) {
                    return p.points.size() == static_cast<size_t>(vertexCount);
                });
        }

        std::cout << result << std::endl;
    }
    catch (const std::invalid_argument&) {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
    catch (const std::out_of_range&) {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void cmdPerms(const std::vector<Polygon>& polygons, const Polygon& target) {
    if (target.points.size() < 3) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }

    int count = std::count_if(polygons.begin(), polygons.end(),
        [&target](const Polygon& p) {
            return isPermutation(p, target);
        });
    std::cout << count << std::endl;
}

void cmdRightShapes(const std::vector<Polygon>& polygons) {
    int count = std::count_if(polygons.begin(), polygons.end(), hasRightAngle);
    std::cout << count << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return EXIT_FAILURE;
    }

    std::vector<Polygon> polygons = readPolygons(argv[1]);

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "AREA") {
            std::string param;
            iss >> param;
            cmdArea(polygons, param);
        }
        else if (command == "MAX") {
            std::string param;
            iss >> param;
            cmdMax(polygons, param);
        }
        else if (command == "MIN") {
            std::string param;
            iss >> param;
            cmdMin(polygons, param);
        }
        else if (command == "COUNT") {
            std::string param;
            iss >> param;
            cmdCount(polygons, param);
        }
        else if (command == "PERMS") {
            size_t pos = line.find_first_not_of(" \t", command.length());
            if (pos != std::string::npos) {
                std::string polygonStr = line.substr(pos);
                Polygon target = parsePolygonFromString(polygonStr);
                cmdPerms(polygons, target);
            } else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        else if (command == "RIGHTSHAPES") {
            cmdRightShapes(polygons);
        }
        else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}
