#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iomanip>
#include <cmath>
#include <map>
#include <cstdlib>
using namespace std::placeholders;
namespace geometry {
struct Point {
    int x, y;
    bool operator==(const Point& other) const;
};
std::istream& operator>>(std::istream& is, Point& p);
std::ostream& operator<<(std::ostream& os, const Point& p);
struct Polygon {
    std::vector<Point> points;
    double area() const;
    size_t vertexCount() const;
    bool operator==(const Polygon& other) const;
};
std::istream& operator>>(std::istream& is, Polygon& poly);
std::ostream& operator<<(std::ostream& os, const Polygon& poly);
bool isRectangle(const Polygon& poly);
bool isSameShape(const Polygon& a, const Polygon& b);
bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}
std::istream& operator>>(std::istream& is, Point& p) {
    char open, comma, close;
    is >> open >> p.x >> comma >> p.y >> close;
    if (open != '(' || comma != ';' || close != ')') {
        is.setstate(std::ios::failbit);
    }
    return is;
}
std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ";" << p.y << ")";
    return os;
}
double Polygon::area() const {
    if (points.size() < 3) return 0.0;
    double area = 0.0;
    size_t n = points.size();
    for (size_t i = 0; i < n; ++i) {
        const Point& p1 = points[i];
        const Point& p2 = points[(i + 1) % n];
        area += static_cast<double>(p1.x * p2.y - p2.x * p1.y);
    }
    return std::abs(area) / 2.0;
}
size_t Polygon::vertexCount() const {
    return points.size();
}
std::istream& operator>>(std::istream& is, Polygon& poly) {
    size_t numPoints;
    if (!(is >> numPoints)) return is;
    std::vector<Point> points;
    points.reserve(numPoints);
    for (size_t i = 0; i < numPoints; ++i) {
        Point p;
        if (!(is >> p)) {
            is.setstate(std::ios::failbit);
            return is;
        }
        points.push_back(p);
    }
    if (points.size() == numPoints) {
        poly.points = std::move(points);
    } else {
        is.setstate(std::ios::failbit);
    }
    return is;
}
std::ostream& operator<<(std::ostream& os, const Polygon& poly) {
    os << poly.points.size();
    for (const auto& p : poly.points) {
        os << " " << p;
    }
    return os;
}
bool isRectangle(const Polygon& poly) {
    if (poly.vertexCount() != 4) return false;
    const auto& points = poly.points;
    auto getVector = [](const Point& a, const Point& b) -> std::pair<int, int> {
        return {b.x - a.x, b.y - a.y};
    };
    auto dot = [](const std::pair<int, int>& v1, const std::pair<int, int>& v2) -> int {
        return v1.first * v2.first + v1.second * v2.second;
    };
    for (size_t i = 0; i < 4; ++i) {
        const auto& p1 = points[i];
        const auto& p2 = points[(i + 1) % 4];
        const auto& p3 = points[(i + 2) % 4];
        auto v1 = getVector(p1, p2);
        auto v2 = getVector(p2, p3);
        if (dot(v1, v2) != 0) return false;
    }
    return true;
}
bool isSameShape(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) return false;
    size_t n = a.points.size();
    for (size_t offset = 0; offset < n; ++offset) {
        bool match = true;
        for (size_t i = 0; i < n; ++i) {
            if (!(a.points[i] == b.points[(i + offset) % n])) {
                match = false;
                break;
            }
        }
        if (match) return true;
        match = true;
        for (size_t i = 0; i < n; ++i) {
            if (!(a.points[i] == b.points[(offset - i + n) % n])) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}
bool Polygon::operator==(const Polygon& other) const {
    if (points.size() != other.points.size()) return false;
    return isSameShape(*this, other);
}
}
class CommandProcessor {
public:
    using PolygonVector = std::vector<geometry::Polygon>;
    explicit CommandProcessor(PolygonVector polygons);
    void processCommand(const std::string& commandLine);
private:
    PolygonVector polygons_;
    std::map<std::string, std::function<void(std::istringstream&)>> handlers_;
    void initHandlers();
    void handleArea(std::istringstream& args);
    void handleMax(std::istringstream& args);
    void handleMin(std::istringstream& args);
    void handleCount(std::istringstream& args);
    void handleRects(std::istringstream& args);
    void handleSame(std::istringstream& args);
    template<typename Predicate>
    double sumAreaIf(Predicate pred) const;
    template<typename Predicate>
    size_t countIf(Predicate pred) const;
};
CommandProcessor::CommandProcessor(PolygonVector polygons)
    : polygons_(std::move(polygons)) {
    initHandlers();
}
void CommandProcessor::initHandlers() {
    handlers_["AREA"] = std::bind(&CommandProcessor::handleArea, this, _1);
    handlers_["MAX"] = std::bind(&CommandProcessor::handleMax, this, _1);
    handlers_["MIN"] = std::bind(&CommandProcessor::handleMin, this, _1);
    handlers_["COUNT"] = std::bind(&CommandProcessor::handleCount, this, _1);
    handlers_["RECTS"] = std::bind(&CommandProcessor::handleRects, this, _1);
    handlers_["SAME"] = std::bind(&CommandProcessor::handleSame, this, _1);
}
void CommandProcessor::processCommand(const std::string& commandLine) {
    std::istringstream iss(commandLine);
    std::string command;
    iss >> command;
    auto it = handlers_.find(command);
    if (it != handlers_.end()) {
        it->second(iss);
    } else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}
template<typename Predicate>
double CommandProcessor::sumAreaIf(Predicate pred) const {
    if (polygons_.empty()) return 0.0;
    std::vector<double> areas(polygons_.size());
    std::transform(polygons_.cbegin(), polygons_.cend(), areas.begin(),
                   std::bind(&geometry::Polygon::area, _1));
    std::vector<double> filteredAreas;
    filteredAreas.reserve(polygons_.size());
    auto areaIt = areas.cbegin();
    auto polyIt = polygons_.cbegin();
    std::copy_if(areas.cbegin(), areas.cend(), std::back_inserter(filteredAreas),
                 [&pred, &polyIt, &areaIt](double) mutable {
                     bool result = pred(*polyIt);
                     ++polyIt;
                     ++areaIt;
                     return result;
                 });
    return std::accumulate(filteredAreas.cbegin(), filteredAreas.cend(), 0.0);
}
template<typename Predicate>
size_t CommandProcessor::countIf(Predicate pred) const {
    return std::count_if(polygons_.cbegin(), polygons_.cend(), pred);
}
void CommandProcessor::handleArea(std::istringstream& args) {
    std::string param;
    args >> param;
    if (polygons_.empty()) {
        std::cout << "0.0" << std::endl;
        return;
    }
    if (param == "ODD") {
        double sum = sumAreaIf(std::bind(std::equal_to<size_t>(),
                    std::bind(std::modulus<size_t>(), std::bind(&geometry::Polygon::vertexCount, _1), 2), 1));
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    } else if (param == "EVEN") {
        double sum = sumAreaIf(std::bind(std::equal_to<size_t>(),
                    std::bind(std::modulus<size_t>(), std::bind(&geometry::Polygon::vertexCount, _1), 2), 0));
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    } else if (param == "MEAN") {
        std::vector<double> areas(polygons_.size());
        std::transform(polygons_.cbegin(), polygons_.cend(), areas.begin(),
                       std::bind(&geometry::Polygon::area, _1));
        double sum = std::accumulate(areas.cbegin(), areas.cend(), 0.0);
        std::cout << std::fixed << std::setprecision(1) << sum / polygons_.size() << std::endl;
    } else {
        size_t vertexCount = std::stoul(param);
        double sum = sumAreaIf(std::bind(std::equal_to<size_t>(),
                    std::bind(&geometry::Polygon::vertexCount, _1), vertexCount));
        std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
    }
}
void CommandProcessor::handleMax(std::istringstream& args) {
    std::string param;
    args >> param;
    if (polygons_.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (param == "AREA") {
        auto maxIt = std::max_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<double>(), std::bind(&geometry::Polygon::area, _1),
                     std::bind(&geometry::Polygon::area, _2)));
        std::cout << std::fixed << std::setprecision(1) << maxIt->area() << std::endl;
    } else if (param == "VERTEXES") {
        auto maxIt = std::max_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<size_t>(), std::bind(&geometry::Polygon::vertexCount, _1),
                     std::bind(&geometry::Polygon::vertexCount, _2)));
        std::cout << maxIt->vertexCount() << std::endl;
    } else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}
void CommandProcessor::handleMin(std::istringstream& args) {
    std::string param;
    args >> param;
    if (polygons_.empty()) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (param == "AREA") {
        auto minIt = std::min_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<double>(), std::bind(&geometry::Polygon::area, _1),
                     std::bind(&geometry::Polygon::area, _2)));
        std::cout << std::fixed << std::setprecision(1) << minIt->area() << std::endl;
    } else if (param == "VERTEXES") {
        auto minIt = std::min_element(polygons_.cbegin(), polygons_.cend(),
            std::bind(std::less<size_t>(), std::bind(&geometry::Polygon::vertexCount, _1),
                     std::bind(&geometry::Polygon::vertexCount, _2)));
        std::cout << minIt->vertexCount() << std::endl;
    } else {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}
void CommandProcessor::handleCount(std::istringstream& args) {
    std::string param;
    args >> param;
    if (param == "ODD") {
        size_t count = countIf(std::bind(std::equal_to<size_t>(),
                    std::bind(std::modulus<size_t>(), std::bind(&geometry::Polygon::vertexCount, _1), 2), 1));
        std::cout << count << std::endl;
    } else if (param == "EVEN") {
        size_t count = countIf(std::bind(std::equal_to<size_t>(),
                    std::bind(std::modulus<size_t>(), std::bind(&geometry::Polygon::vertexCount, _1), 2), 0));
        std::cout << count << std::endl;
    } else {
        size_t vertexCount = std::stoul(param);
        size_t count = countIf(std::bind(std::equal_to<size_t>(),
                    std::bind(&geometry::Polygon::vertexCount, _1), vertexCount));
        std::cout << count << std::endl;
    }
}
void CommandProcessor::handleRects(std::istringstream& args) {
    size_t count = countIf(std::bind(&geometry::isRectangle, _1));
    std::cout << count << std::endl;
}
void CommandProcessor::handleSame(std::istringstream& args) {
    geometry::Polygon target;
    if (!(args >> target)) {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    size_t count = countIf(std::bind(&geometry::isSameShape, target, _1));
    std::cout << count << std::endl;
}
int main(int argc, char* argv[]) {
    try {
        if (argc != 2) {
            std::cerr << "Error: Filename parameter is required" << std::endl;
            return EXIT_FAILURE;
        }
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Cannot open file " << argv[1] << std::endl;
            return EXIT_FAILURE;
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
        return EXIT_SUCCESS;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Error: Unknown exception occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
