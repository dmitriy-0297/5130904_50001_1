#include "compositeShape.h"
#include <limits>
#include <algorithm>

void CompositeShape::addShape(std::unique_ptr<Shape> shape)
{
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const
{
    double sum = 0;

    for (const auto& s : shapes_)
        sum += s->getArea();

    return sum;
}

Point CompositeShape::getCenter() const
{
    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& s : shapes_)
    {
        Point c = s->getCenter();

        minX = std::min(minX, c.x);
        minY = std::min(minY, c.y);
        maxX = std::max(maxX, c.x);
        maxY = std::max(maxY, c.y);
    }

    return {(minX + maxX)/2, (minY + maxY)/2};
}

void CompositeShape::move(double dx, double dy)
{
    for (auto& s : shapes_)
        s->move(dx, dy);
}

void CompositeShape::scale(double k)
{
    Point center = getCenter();

    for (auto& s : shapes_)
    {
        Point c = s->getCenter();

        double dx = (c.x - center.x) * (k - 1);
        double dy = (c.y - center.y) * (k - 1);

        s->move(dx, dy);
        s->scale(k);
    }
}

std::string CompositeShape::getName() const
{
    return "COMPOSITE";
}
