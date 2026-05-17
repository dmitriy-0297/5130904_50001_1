#include "polygon.h"
#include <cmath>
#include <sstream>
#include <algorithm>

bool Point::operator==(const Point& other) const {
    return x == other.x && y == other.y;
}

bool Polygon::operator==(const Polygon& other) const {
    if (points.size() != other.points.size()) return false;
    return std::equal(points.begin(), points.end(), other.points.begin());
}

double polygonArea(const Polygon& poly) {
    if (poly.points.size() < 3) return 0.0;
    double area = 0.0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i) {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % n];
        area += p1.x * p2.y - p2.x * p1.y;
    }
    return std::abs(area) / 2.0;
}

static int orientation(const Point& a, const Point& b, const Point& c) {
    long long val = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

static bool onSegment(const Point& p, const Point& q, const Point& r) {
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;
    return false;
}

static bool segmentsIntersect(const Point& p1, const Point& p2, const Point& p3, const Point& p4) {
    int o1 = orientation(p1, p2, p3);
    int o2 = orientation(p1, p2, p4);
    int o3 = orientation(p3, p4, p1);
    int o4 = orientation(p3, p4, p2);
    if (o1 != o2 && o3 != o4) return true;
    if (o1 == 0 && onSegment(p1, p3, p2)) return true;
    if (o2 == 0 && onSegment(p1, p4, p2)) return true;
    if (o3 == 0 && onSegment(p3, p1, p4)) return true;
    if (o4 == 0 && onSegment(p3, p2, p4)) return true;
    return false;
}

static bool pointInPolygon(const Point& p, const Polygon& poly) {
    bool inside = false;
    size_t n = poly.points.size();
    for (size_t i = 0, j = n - 1; i < n; j = i++) {
        const Point& pi = poly.points[i];
        const Point& pj = poly.points[j];
        if (((pi.y > p.y) != (pj.y > p.y)) &&
            (p.x < (pj.x - pi.x) * (p.y - pi.y) / (pj.y - pi.y) + pi.x))
            inside = !inside;
    }
    return inside;
}

bool polygonsIntersect(const Polygon& a, const Polygon& b) {
    for (size_t i = 0; i < a.points.size(); ++i) {
        const Point& a1 = a.points[i];
        const Point& a2 = a.points[(i + 1) % a.points.size()];
        for (size_t j = 0; j < b.points.size(); ++j) {
            const Point& b1 = b.points[j];
            const Point& b2 = b.points[(j + 1) % b.points.size()];
            if (segmentsIntersect(a1, a2, b1, b2)) return true;
        }
    }
    if (pointInPolygon(a.points[0], b)) return true;
    if (pointInPolygon(b.points[0], a)) return true;
    return false;
}

Polygon parsePolygon(const std::string& str) {
    Polygon poly;
    std::istringstream iss(str);
    int n;
    if (!(iss >> n)) return poly;
    if (n < 3) return poly;              // не многоугольник
    for (int i = 0; i < n; ++i) {
        char c1, c2, c3;
        int x, y;
        if (!(iss >> c1 >> x >> c2 >> y >> c3) || c1 != '(' || c2 != ';' || c3 != ')') {
            poly.points.clear();
            return poly;
        }
        poly.points.push_back({x, y});
    }
    // после чтения ровно n вершин в строке не должно быть мусора
    std::string rest;
    if (iss >> rest) {
        poly.points.clear();
    }
    return poly;
}
