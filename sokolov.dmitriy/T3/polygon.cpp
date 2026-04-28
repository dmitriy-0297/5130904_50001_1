#include <cstdlib>
#include "polygon.hpp"

double Polygon::area() const {

    if (points.size() < 3) {
        return 0.0;
    }

    double area = 0.0;

    for (size_t i = 0; i < points.size(); ++i) {

        const Point &p1 = points[i];
        const Point &p2 = points[(i + 1) % points.size()];

        area += (p1.x * p2.y) - (p1.y * p2.x);
    }

    return std::abs(area) / 2.0;

}
