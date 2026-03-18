#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

class Rectangle : public Shape
{
private:
    Point leftBottom_;
    Point rightTop_;

public:
    Rectangle(Point lb, Point rt);

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double k) override;

    std::string getName() const override;
};

#endif
