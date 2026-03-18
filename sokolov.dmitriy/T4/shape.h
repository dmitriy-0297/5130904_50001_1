#ifndef SHAPE_H
#define SHAPE_H
#include "point.h"
#include <string>
#include <vector>

class Shape {
public:

    virtual double getArea() const = 0;

    virtual Point getCenter() const = 0;

    virtual void move(const double, const double) = 0;

    virtual void scale(const double) = 0;

    virtual std::string getName() const = 0;

    virtual std::vector<Point> getPoints() const = 0;

    virtual ~Shape() = default;

};

#endif
