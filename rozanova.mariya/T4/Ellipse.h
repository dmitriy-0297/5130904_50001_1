#ifndef ELLIPSE_H
#define ELLIPSE_H
#include "Shape.h"

class Ellipse : public Shape
{
private:
    Point center_;
    double rx_ = 0.0;
    double ry_ = 0.0;
public:
    Ellipse(Point center, double rx, double ry);

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
