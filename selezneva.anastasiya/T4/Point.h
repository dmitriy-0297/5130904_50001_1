#ifndef POINT_H
#define POINT_H

#include <iostream>

struct Point {
  double x = 0.0;
  double y = 0.0;

  Point() = default;
  Point(double x, double y) : x(x), y(y) {};

  friend std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ")";
    return os;
  }
};
#endif
