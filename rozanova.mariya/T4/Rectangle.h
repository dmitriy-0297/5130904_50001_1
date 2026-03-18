#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "Shape.h"

class Rectangle: public Shape
{
private:
    Point left_;
    Point right_;
public:
    Rectangle(Point left, Point right);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double k) override;
    std::string getName() const override;

    Point getBottomLeft() const override;
    Point getTopRight() const override;

    void print() const override;
};

#endif
