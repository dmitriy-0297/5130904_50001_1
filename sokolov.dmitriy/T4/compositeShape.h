#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H
#include "shape.h"
#include "point.h"
#include <string>
#include <vector>
#include <memory>

class CompositeShape : public Shape {
public:

    CompositeShape();

    CompositeShape(const CompositeShape&) = delete;

    CompositeShape& operator=(const CompositeShape&) = delete;

    void addShape(std::unique_ptr<Shape>);

    void removeShape(const size_t);

    const Shape* operator[](const size_t) const;

    size_t size() const;

    virtual double getArea() const override;

    virtual Point getCenter() const override;

    virtual void move(const double, const double) override;

    virtual void scale(const double) override;

    virtual std::string getName() const override;

    virtual std::vector<Point> getPoints() const override;

    virtual ~CompositeShape() = default;

private:

    std::vector<std::unique_ptr<Shape>> shapes_;

};

#endif
