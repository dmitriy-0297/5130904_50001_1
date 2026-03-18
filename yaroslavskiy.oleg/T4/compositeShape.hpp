
#ifndef COMPOSITE_SHAPE_HPP
#define COMPOSITE_SHAPE_HPP

#include "shape.hpp"
#include <vector>

class CompositeShape : public Shape {

private:
    std::vector<Shape*> shapes;

public:
    CompositeShape();
    ~CompositeShape();

    void addShape(Shape* shape);
    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double coef) override;
    std::string getName() const override;
    void printDetails() const;

};

#endif



