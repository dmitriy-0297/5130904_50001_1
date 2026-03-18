#ifndef SQUARE_H
#define SQUARE_H

#include "shape.h"

//квадрат вариант 5
class Square : public Shape
{
public:

    Square(const Point& bottomLeft, double side);

    virtual ~Square();

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::pair<Point, Point> getBoundingBox() const override;
    void print(std::ostream& os) const override;

private:
    Point bottomLeft_;
    double side_;
};

#endif
