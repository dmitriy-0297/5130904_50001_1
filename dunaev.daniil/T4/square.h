#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"

class Square : public Shape
{
private:
    Point leftBottom_;
    double side_;

public:
    Square(Point lb, double side);

    double getArea() const override;
    Point getCenter() const override;

    void move(double dx, double dy) override;
    void scale(double k) override;

    std::string getName() const override;
};

#endif
