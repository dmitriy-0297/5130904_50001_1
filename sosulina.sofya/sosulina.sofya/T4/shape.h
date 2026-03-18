#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>
#include <utility>
#include <iostream>

class Shape
{
public:

    Shape();

    virtual ~Shape();

    virtual double getArea() const = 0;

    virtual Point getCenter() const = 0;

    virtual void move(const Point& delta) = 0;

    virtual void scale(double factor) = 0;

    virtual std::string getName() const = 0;

    virtual std::pair<Point, Point> getBoundingBox() const = 0;

    virtual void print(std::ostream& os) const = 0;

    int getId() const;

protected:
    int id_;

private:

    static int counter_;
};

#endif
