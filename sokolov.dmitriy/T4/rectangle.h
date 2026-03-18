#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "shape.h"
#include "point.h"
#include <string>

class Rectangle : public Shape {
public:

    Rectangle(const Point&, const Point&);

    virtual double getArea() const override;

    virtual Point getCenter() const override;

    virtual void move(const double, const double) override;

    virtual void scale(const double) override;

    virtual std::string getName() const override;

    virtual std::vector<Point> getPoints() const override;

    virtual ~Rectangle() = default;

private:

    Point leftDown_;
    Point rightUp_;

};

#endif
