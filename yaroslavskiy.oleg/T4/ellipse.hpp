
#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP

#include "shape.hpp"

class Ellipse : public Shape {

private:

    Point centerO;
    double radiusX;
    double radiusY;

public:

    Ellipse(const Point& o, double rx, double ry);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    std::string getName() const override;

};

#endif




