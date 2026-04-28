#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <string>

struct Point {
    int x, y;
};

struct Polygon {
    std::vector<Point> points;
};

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon& a, const Polygon& b) {
    return a.points == b.points;
}

double getArea(const Polygon& p) {
    double area = 0;
    for (size_t i = 0; i < p.points.size(); ++i) {
        const auto& a = p.points[i];
        const auto& b = p.points[(i + 1) % p.points.size()];
        area += (a.x * b.y - b.x * a.y);
    }
    return std::abs(area) / 2.0;
}

bool isEven(const Polygon& p) {
    return p.points.size() % 2 == 0;
}

bool isOdd(const Polygon& p) {
    return p.points.size() % 2 != 0;
}

bool isRectangle(const Polygon& p) {
    if (p.points.size() != 4) return false;

    auto dot = [](const Point& a, const Point& b, const Point& c) {
        int x1 = b.x - a.x;
        int y1 = b.y - a.y;
        int x2 = c.x - b.x;
        int y2 = c.y - b.y;
        return x1 * x2 + y1 * y2;
        };

    auto dist2 = [](const Point& a, const Point& b) {
        int dx = a.x - b.x;
        int dy = a.y - b.y;
        return dx * dx + dy * dy;
        };

    for (int i = 0; i < 4; ++i) {
        if (dist2(p.points[i], p.points[(i + 1) % 4]) == 0)
            return false;
    }

    bool rightAngles =
        dot(p.points[0], p.points[1], p.points[2]) == 0 &&
        dot(p.points[1], p.points[2], p.points[3]) == 0 &&
        dot(p.points[2], p.points[3], p.points[0]) == 0 &&
        dot(p.points[3], p.points[0], p.points[1]) == 0;

    if (!rightAngles) return false;

    return dist2(p.points[0], p.points[1]) == dist2(p.points[2], p.points[3]) &&
        dist2(p.points[1], p.points[2]) == dist2(p.points[3], p.points[0]);
}

bool matchWithShift(const std::vector<Point>& a, const std::vector<Point>& b) {
    int n = a.size();

    for (int shift = 0; shift < n; ++shift) {
        int dx = b[shift].x - a[0].x;
        int dy = b[shift].y - a[0].y;

        bool ok = true;
        for (int i = 0; i < n; ++i) {
            const Point& p1 = a[i];
            const Point& p2 = b[(i + shift) % n];
            if (p1.x + dx != p2.x || p1.y + dy != p2.y) {
                ok = false;
                break;
            }
        }
        if (ok) return true;
    }
    return false;
}

bool isSameShape(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) return false;

    if (matchWithShift(a.points, b.points)) return true;

    std::vector<Point> rev = b.points;
    std::reverse(rev.begin(), rev.end());

    return matchWithShift(a.points, rev);
}

bool safeStoi(const std::string& s, int& result) {
    try {
        size_t pos;
        result = std::stoi(s, &pos);
        return pos == s.size();
    }
    catch (...) {
        return false;
    }
}

bool hasExtra(std::istringstream& iss) {
    std::string extra;
    return static_cast<bool>(iss >> extra);
}

bool parsePolygon(std::istream& in, Polygon& poly) {
    int n;
    if (!(in >> n) || n < 3) return false;

    std::vector<Point> pts(n);
    for (int i = 0; i < n; ++i) {
        char c;
        if (!(in >> c) || c != '(') return false;
        if (!(in >> pts[i].x)) return false;
        if (!(in >> c) || c != ';') return false;
        if (!(in >> pts[i].y)) return false;
        if (!(in >> c) || c != ')') return false;
    }

    poly.points = pts;
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "No file\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> data;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Polygon p;
        if (parsePolygon(iss, p)) {
            data.push_back(p);
        }
    }

    std::cout << std::fixed << std::setprecision(1);

    while (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "AREA") {
            std::string arg;
            iss >> arg;
            if (hasExtra(iss)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            if (arg == "EVEN") {
                double res = std::accumulate(data.begin(), data.end(), 0.0,
                    [](double s, const Polygon& p) {
                        return isEven(p) ? s + getArea(p) : s;
                    });
                std::cout << res << '\n';
            }
            else if (arg == "ODD") {
                double res = std::accumulate(data.begin(), data.end(), 0.0,
                    [](double s, const Polygon& p) {
                        return isOdd(p) ? s + getArea(p) : s;
                    });
                std::cout << res << '\n';
            }
            else if (arg == "MEAN") {
                if (data.empty()) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                double sum = std::accumulate(data.begin(), data.end(), 0.0,
                    [](double s, const Polygon& p) {
                        return s + getArea(p);
                    });
                std::cout << sum / data.size() << '\n';
            }
            else {
                int n;
                if (!safeStoi(arg, n) || n < 3) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }

                double res = std::accumulate(data.begin(), data.end(), 0.0,
                    [n](double s, const Polygon& p) {
                        return p.points.size() == n ? s + getArea(p) : s;
                    });
                std::cout << res << '\n';
            }
        }
        else if (cmd == "MAX" || cmd == "MIN") {
            std::string arg;
            iss >> arg;
            if (hasExtra(iss) || data.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            auto cmpArea = [](const Polygon& a, const Polygon& b) {
                return getArea(a) < getArea(b);
                };
            auto cmpVert = [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
                };

            if (arg == "AREA") {
                auto it = (cmd == "MAX")
                    ? std::max_element(data.begin(), data.end(), cmpArea)
                    : std::min_element(data.begin(), data.end(), cmpArea);
                std::cout << getArea(*it) << '\n';
            }
            else if (arg == "VERTEXES") {
                auto it = (cmd == "MAX")
                    ? std::max_element(data.begin(), data.end(), cmpVert)
                    : std::min_element(data.begin(), data.end(), cmpVert);
                std::cout << it->points.size() << '\n';
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (cmd == "COUNT") {
            std::string arg;
            iss >> arg;
            if (hasExtra(iss)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            if (arg == "EVEN") {
                std::cout << std::count_if(data.begin(), data.end(), isEven) << '\n';
            }
            else if (arg == "ODD") {
                std::cout << std::count_if(data.begin(), data.end(), isOdd) << '\n';
            }
            else {
                int n;
                if (!safeStoi(arg, n) || n < 3) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }

                std::cout << std::count_if(data.begin(), data.end(),
                    [n](const Polygon& p) {
                        return p.points.size() == n;
                    }) << '\n';
            }
        }
        else if (cmd == "RECTS") {
            if (hasExtra(iss)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            std::cout << std::count_if(data.begin(), data.end(), isRectangle) << '\n';
        }
        else if (cmd == "SAME") {
            Polygon p;
            if (!parsePolygon(iss, p) || hasExtra(iss)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            std::cout << std::count_if(data.begin(), data.end(),
                [&](const Polygon& x) {
                    return isSameShape(x, p);
                }) << '\n';
        }
        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
