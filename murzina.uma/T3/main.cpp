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
    int x;
    int y;
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

std::istream& operator>>(std::istream& is, Point& p) {
    char open, comma, close;
    if (is >> open && open == '(') {
        if (is >> p.x >> comma && comma == ';') {
            if (is >> p.y >> close && close == ')') {
                return is;
            }
        }
    }
    is.setstate(std::ios::failbit);
    return is;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << ";" << p.y << ")";
    return os;
}

struct Polygon {
    std::vector<Point> points;

    double area() const {
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

    size_t vertexCount() const {
        return points.size();
    }

    bool operator==(const Polygon& other) const {
        return points == other.points;
    }
};

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

long long cross(const Point& a, const Point& b, const Point& c) {
    long long x1 = static_cast<long long>(b.x) - a.x;
    long long y1 = static_cast<long long>(b.y) - a.y;
    long long x2 = static_cast<long long>(c.x) - a.x;
    long long y2 = static_cast<long long>(c.y) - a.y;
    return x1 * y2 - y1 * x2;
}

bool isValidPolygon(const Polygon& poly) {
    if (poly.points.size() < 3) {
        return false;
    }

    std::vector<Point> pts = poly.points;
    std::sort(pts.begin(), pts.end());

    if (std::adjacent_find(pts.begin(), pts.end()) != pts.end()) {
        return false;
    }

    for (size_t i = 0; i < pts.size(); ++i) {
        for (size_t j = i + 1; j < pts.size(); ++j) {
            for (size_t k = j + 1; k < pts.size(); ++k) {
                if (cross(pts[i], pts[j], pts[k]) == 0) {
                    return false;
                }
            }
        }
    }

    return true;
}

std::vector<Point> normalizeByShift(const Polygon& poly) {
    std::vector<Point> result = poly.points;
    auto minIt = std::min_element(result.begin(), result.end());
    int dx = minIt->x;
    int dy = minIt->y;
    std::transform(result.begin(), result.end(), result.begin(),
        [dx, dy](const Point& p) {
            return Point(p.x - dx, p.y - dy);
        });
    std::sort(result.begin(), result.end());
    return result;
}

bool isSameShape(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) {
        return false;
    }
    return normalizeByShift(a) == normalizeByShift(b);
}

bool isRectangle(const Polygon& poly) {
    if (poly.vertexCount() != 4) return false;
    const auto& points = poly.points;

    auto getVector = [](const Point& a, const Point& b) -> std::pair<long long, long long> {
        return {static_cast<long long>(b.x) - a.x, static_cast<long long>(b.y) - a.y};
    };

    auto dot = [](const std::pair<long long, long long>& v1, const std::pair<long long, long long>& v2) -> long long {
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

}

bool readPolygonArg(std::istringstream& args, geometry::Polygon& target) {
    if (!(args >> target)) {
        return false;
    }
    args >> std::ws;
    return args.eof() && geometry::isValidPolygon(target);
}

class CommandProcessor {
public:
    using PolygonVector = std::vector<geometry::Polygon>;

    explicit CommandProcessor(PolygonVector polygons) : polygons_(std::move(polygons)) {
        initHandlers();
    }

    void processCommand(const std::string& commandLine) {
        std::istringstream iss(commandLine);
        std::string command;
        iss >> command;

        if (command.empty()) return;

        auto it = handlers_.find(command);
        if (it != handlers_.end()) {
            it->second(iss);
        } else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }

private:
    PolygonVector polygons_;
    std::map<std::string, std::function<void(std::istringstream&)>> handlers_;

    void initHandlers() {
        handlers_["AREA"] = std::bind(&CommandProcessor::handleArea, this, _1);
        handlers_["MAX"] = std::bind(&CommandProcessor::handleMax, this, _1);
        handlers_["MIN"] = std::bind(&CommandProcessor::handleMin, this, _1);
        handlers_["COUNT"] = std::bind(&CommandProcessor::handleCount, this, _1);
        handlers_["RECTS"] = std::bind(&CommandProcessor::handleRects, this, _1);
        handlers_["MAXSEQ"] = std::bind(&CommandProcessor::handleMaxSeq, this, _1);
    }

    template<typename Predicate>
    double sumAreaIf(Predicate pred) const {
        if (polygons_.empty()) return 0.0;

        double sum = 0.0;
        for (const auto& poly : polygons_) {
            if (pred(poly)) {
                sum += poly.area();
            }
        }
        return sum;
    }

    template<typename Predicate>
    size_t countIf(Predicate pred) const {
        return std::count_if(polygons_.cbegin(), polygons_.cend(), pred);
    }

    void handleArea(std::istringstream& args) {
        std::string param;
        args >> param;

        if (param.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }

        if (polygons_.empty()) {
            if (param == "MEAN") {
                std::cout << "<INVALID COMMAND>" << std::endl;
            } else if (param == "ODD" || param == "EVEN") {
                std::cout << "0.0" << std::endl;
            } else {
                try {
                    size_t vertexCount = std::stoul(param);
                    if (vertexCount < 3) {
                        std::cout << "<INVALID COMMAND>" << std::endl;
                    } else {
                        std::cout << "0.0" << std::endl;
                    }
                } catch (...) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                }
            }
            return;
        }

        if (param == "ODD") {
            double sum = sumAreaIf([](const geometry::Polygon& p) {
                return p.vertexCount() % 2 != 0;
            });
            std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
        } else if (param == "EVEN") {
            double sum = sumAreaIf([](const geometry::Polygon& p) {
                return p.vertexCount() % 2 == 0;
            });
            std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
        } else if (param == "MEAN") {
            double sum = 0.0;
            for (const auto& p : polygons_) {
                sum += p.area();
            }
            std::cout << std::fixed << std::setprecision(1) << sum / polygons_.size() << std::endl;
        } else {
            try {
                size_t vertexCount = std::stoul(param);
                if (vertexCount < 3) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    return;
                }
                double sum = sumAreaIf([vertexCount](const geometry::Polygon& p) {
                    return p.vertexCount() == vertexCount;
                });
                std::cout << std::fixed << std::setprecision(1) << sum << std::endl;
            } catch (...) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
    }

    void handleMax(std::istringstream& args) {
        std::string param;
        args >> param;

        if (param.empty() || polygons_.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }

        if (param == "AREA") {
            auto maxIt = std::max_element(polygons_.cbegin(), polygons_.cend(),
                [](const geometry::Polygon& a, const geometry::Polygon& b) {
                    return a.area() < b.area();
                });
            std::cout << std::fixed << std::setprecision(1) << maxIt->area() << std::endl;
        } else if (param == "VERTEXES") {
            auto maxIt = std::max_element(polygons_.cbegin(), polygons_.cend(),
                [](const geometry::Polygon& a, const geometry::Polygon& b) {
                    return a.vertexCount() < b.vertexCount();
                });
            std::cout << maxIt->vertexCount() << std::endl;
        } else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }

    void handleMin(std::istringstream& args) {
        std::string param;
        args >> param;

        if (param.empty() || polygons_.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }

        if (param == "AREA") {
            auto minIt = std::min_element(polygons_.cbegin(), polygons_.cend(),
                [](const geometry::Polygon& a, const geometry::Polygon& b) {
                    return a.area() < b.area();
                });
            std::cout << std::fixed << std::setprecision(1) << minIt->area() << std::endl;
        } else if (param == "VERTEXES") {
            auto minIt = std::min_element(polygons_.cbegin(), polygons_.cend(),
                [](const geometry::Polygon& a, const geometry::Polygon& b) {
                    return a.vertexCount() < b.vertexCount();
                });
            std::cout << minIt->vertexCount() << std::endl;
        } else {
            std::cout << "<INVALID COMMAND>" << std::endl;
        }
    }

    void handleCount(std::istringstream& args) {
        std::string param;
        args >> param;

        if (param.empty()) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }

        if (param == "ODD") {
            size_t count = countIf([](const geometry::Polygon& p) {
                return p.vertexCount() % 2 != 0;
            });
            std::cout << count << std::endl;
        } else if (param == "EVEN") {
            size_t count = countIf([](const geometry::Polygon& p) {
                return p.vertexCount() % 2 == 0;
            });
            std::cout << count << std::endl;
        } else {
            try {
                size_t vertexCount = std::stoul(param);
                if (vertexCount < 3) {
                    std::cout << "<INVALID COMMAND>" << std::endl;
                    return;
                }
                size_t count = countIf([vertexCount](const geometry::Polygon& p) {
                    return p.vertexCount() == vertexCount;
                });
                std::cout << count << std::endl;
            } catch (...) {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
    }

    void handleRects(std::istringstream&) {
        size_t count = countIf(geometry::isRectangle);
        std::cout << count << std::endl;
    }

    void handleMaxSeq(std::istringstream& args) {
        geometry::Polygon target;

        if (!readPolygonArg(args, target)) {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }

        size_t maxSeq = 0;
        size_t currentSeq = 0;

        for (const auto& poly : polygons_) {
            if (poly == target) {
                ++currentSeq;
                if (currentSeq > maxSeq) {
                    maxSeq = currentSeq;
                }
            } else {
                currentSeq = 0;
            }
        }

        std::cout << maxSeq << std::endl;
    }
};

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
            if (line.empty()) {
                continue;
            }

            std::istringstream iss(line);
            geometry::Polygon poly;

            if (iss >> poly) {
                iss >> std::ws;

                if (iss.eof() && geometry::isValidPolygon(poly)) {
                    polygons.push_back(poly);
                }
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
