#ifndef COMPOSITE_SHAPE_H
#define COMPOSITE_SHAPE_H

#include "shape.h"
#include <vector>
#include <memory>

//соотавная фигура (контейнер для других фигур)
class CompositeShape : public Shape
{
public:
    CompositeShape();

    virtual ~CompositeShape();

    void addShape(std::shared_ptr<Shape> shape);

    double getArea() const override;
    Point getCenter() const override;
    void move(const Point& delta) override;
    void scale(double factor) override;
    std::string getName() const override;
    std::pair<Point, Point> getBoundingBox() const override;
    void print(std::ostream& os) const override;

private:
    std::vector<std::shared_ptr<Shape>> shapes_;
};

#endif
