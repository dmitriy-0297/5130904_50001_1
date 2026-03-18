
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

class Rectangle : public Shape {

private:

    Point bottomLeft;
    Point topRight;

public:

    Rectangle(const Point& t1, const Point& t2);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    std::string getName() const override;

};

#endif




