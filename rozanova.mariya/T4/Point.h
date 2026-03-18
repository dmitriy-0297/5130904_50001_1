#ifndef POINT_H
#define POINT_H

struct Point {
    double x_ = 0.0;
    double y_ = 0.0;

    Point(double x, double y): x_(x), y_(y) {}
};

#endif
