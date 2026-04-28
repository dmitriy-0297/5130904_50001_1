#include "Geometry.hpp"
#include <algorithm>
#include <sstream>
#include <cmath>
#include <functional>

namespace geometry {

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) const {
    return !(*this == other);
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

bool Polygon::operator==(const Polygon& other) const {
    if (points.size() != other.points.size()) return false;
    return isSameShape(*this, other);
}

bool Polygon::operator!=(const Polygon& other) const {
    return !(*this == other);
}

std::istream& operator>>(std::istream& is, Polygon& poly) {
    size_t numPoints;
    if (!(is >> numPoints)) {
        return is;
    }
    
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

double calculateArea(const Polygon& poly) {
    return poly.area();
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
        
        if (dot(v1, v2) != 0) {
            return false;
        }
    }
    
    return true;
}

bool containsRightAngle(const Polygon& poly) {
    if (poly.vertexCount() < 3) return false;
    
    const auto& points = poly.points;
    size_t n = points.size();
    
    auto getVector = [](const Point& a, const Point& b) -> std::pair<int, int> {
        return {b.x - a.x, b.y - a.y};
    };
    
    auto dot = [](const std::pair<int, int>& v1, const std::pair<int, int>& v2) -> int {
        return v1.first * v2.first + v1.second * v2.second;
    };
    
    for (size_t i = 0; i < n; ++i) {
        const auto& p1 = points[i];
        const auto& p2 = points[(i + 1) % n];
        const auto& p3 = points[(i + 2) % n];
        
        auto v1 = getVector(p1, p2);
        auto v2 = getVector(p2, p3);
        
        if (dot(v1, v2) == 0) {
            return true;
        }
    }
    
    return false;
}

bool isSameShape(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) return false;
    
    size_t n = a.points.size();
    
    for (size_t offset = 0; offset < n; ++offset) {
        bool match = true;
        
        for (size_t i = 0; i < n; ++i) {
            if (a.points[i] != b.points[(i + offset) % n]) {
                match = false;
                break;
            }
        }
        
        if (match) return true;
        
        match = true;
        for (size_t i = 0; i < n; ++i) {
            if (a.points[i] != b.points[(offset - i + n) % n]) {
                match = false;
                break;
            }
        }
        
        if (match) return true;
    }
    
    return false;
}

bool polygonsEqual(const Polygon& a, const Polygon& b) {
    return a == b;
}

} // namespace geometry
