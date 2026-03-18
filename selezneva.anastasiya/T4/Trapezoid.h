#ifndef TRAPEZOID_H
#define TRAPEZOID_H

#include "Shape.h"

class Trapezoid : public Shape {
private:
    Point bottomLeft_ = Point(0.0, 0.0);
    double bottomBase_ = 1.0;
    double topBase_ = 1.0;
    double height_ = 1.0;

public:
    Trapezoid(const Point& bottomLeft, double bottomBase, double topBase, double height);
    ~Trapezoid() override = default;
    Trapezoid(const Trapezoid& other) = default;
    Trapezoid& operator=(const Trapezoid& other) = default;
    Trapezoid(Trapezoid&& other) = default;
    Trapezoid& operator=(Trapezoid&& other) = default;

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;

    Point getBottomLeft() const;
    Point getBottomRight() const;
    Point getTopLeft() const;
    Point getTopRight() const;
};

#endif
