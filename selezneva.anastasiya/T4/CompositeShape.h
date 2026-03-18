#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H

#include "Shape.h"
#include <memory>
#include <vector>

class CompositeShape : public Shape {
private:
    std::vector<std::unique_ptr<Shape>> shapes_;
    void getShapeBoundaries(Point& bottomLeft, Point& topRight) const;

public:
    CompositeShape();
    ~CompositeShape() override = default;
    CompositeShape(const CompositeShape&) = delete;
    CompositeShape& operator=(const CompositeShape&) = delete;
    CompositeShape(CompositeShape&& other) = default;
    CompositeShape& operator=(CompositeShape&& other) = default;

    void addShape(std::unique_ptr<Shape> shape);

    double getArea() const override;
    Point getCenter() const override;
    void move(double dx, double dy) override;
    void scale(double factor) override;

    size_t getShapeCount() const;
    const Shape* getShape(size_t index) const;

    void print(std::ostream& os) const override;
};

#endif
