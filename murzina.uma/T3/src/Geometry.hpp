#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <vector>
#include <iostream>
#include <cmath>

namespace geometry {

struct Point {
    int x, y;
    
    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
    friend std::istream& operator>>(std::istream& is, Point& p);
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

struct Polygon {
    std::vector<Point> points;
    
    double area() const;
    size_t vertexCount() const;
    
    bool operator==(const Polygon& other) const;
    bool operator!=(const Polygon& other) const;
    
    friend std::istream& operator>>(std::istream& is, Polygon& poly);
    friend std::ostream& operator<<(std::ostream& os, const Polygon& poly);
};

double calculateArea(const Polygon& poly);
bool isRectangle(const Polygon& poly);
bool containsRightAngle(const Polygon& poly);
bool isSameShape(const Polygon& a, const Polygon& b);
bool polygonsEqual(const Polygon& a, const Polygon& b);

}

#endif
