#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include "point.hpp"

struct Polygon {
    std::vector<Point> points;

    double area() const;
};

#endif
