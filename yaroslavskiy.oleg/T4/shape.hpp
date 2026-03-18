
#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "point.hpp"
#include <string>

class Shape {

public:

    virtual ~Shape() {}

    virtual double getArea() const = 0;
    virtual Point getCenter() const = 0;
    virtual void move(double dx, double dy) = 0;
    virtual void scale(double coef) = 0;
    virtual std::string getName() const = 0;
};


#endif




