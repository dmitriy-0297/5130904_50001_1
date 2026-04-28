#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>
#include <iostream>

class Shape {
public:
    virtual ~Shape() = default;

    //площадь
    virtual double getArea() const = 0;

    //координаты центра
    virtual Point getCenter() const = 0;

    //перемещение фигуры
    virtual void move(double dx, double dy) = 0;

    virtual void scale(double coefficient) = 0;

    //названия фигуры ("RECTANGLE", "CIRCLE")
    virtual std::string getName() const = 0;

    //ограничивающий прямоугольник (мин и макс)
    virtual void getBounds(Point& minPt, Point& maxPt) const = 0;

    virtual void print(std::ostream& os) const = 0;
};

#endif