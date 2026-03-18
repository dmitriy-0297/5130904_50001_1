#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

//прямоугольник вариант 0
class Rectangle : public Shape
{
public:
    Rectangle(const Point& bottomLeft, const Point& topRight);

    virtual ~Rectangle();

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::pair<Point, Point> getBoundingBox() const override;
    void print(std::ostream& os) const override;

private:
    Point bottomLeft_;
    Point topRight_;
};

#endif
