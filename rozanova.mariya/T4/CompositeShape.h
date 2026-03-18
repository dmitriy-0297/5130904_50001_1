#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H
#include "Shape.h"
#include <vector>
#include <memory>

class CompositeShape: public Shape
{
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
public:
    CompositeShape() = default;

    void addShape(std::unique_ptr<Shape> shape);

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
