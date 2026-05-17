#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>

struct Point {
    int x, y;
    bool operator==(const Point& other) const;
};

struct Polygon {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const;
};

double polygonArea(const Polygon& poly);
bool polygonsIntersect(const Polygon& a, const Polygon& b);
Polygon parsePolygon(const std::string& str);

#endif
