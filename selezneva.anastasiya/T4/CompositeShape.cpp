#include "CompositeShape.h"
#include <algorithm>
#include <limits>
#include <cmath>

CompositeShape::CompositeShape() : Shape("COMPOSITE") {}

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    if (shape) {
        shapes_.push_back(std::move(shape));
    }
}

void CompositeShape::getShapeBoundaries(Point& bottomLeft, Point& topRight) const {
    if (shapes_.empty()) {
        bottomLeft = Point(0, 0);
        topRight = Point(0, 0);
        return;
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = std::numeric_limits<double>::lowest();
    double maxY = std::numeric_limits<double>::lowest();

    for (const auto& shape : shapes_) {
        Point center = shape->getCenter();
        double area = shape->getArea();

        double halfSize = std::sqrt(area) / 2.0;
        minX = std::min(minX, center.x - halfSize);
        minY = std::min(minY, center.y - halfSize);
        maxX = std::max(maxX, center.x + halfSize);
        maxY = std::max(maxY, center.y + halfSize);
    }

    bottomLeft = Point(minX, minY);
    topRight = Point(maxX, maxY);
}

double CompositeShape::getArea() const {
    double totalArea = 0.0;
    for (const auto& shape : shapes_) {
        totalArea += shape->getArea();
    }
    return totalArea;
}

Point CompositeShape::getCenter() const {
    Point bottomLeft;
    Point topRight;
    getShapeBoundaries(bottomLeft, topRight);

    return Point(
        (bottomLeft.x + topRight.x) / 2.0,
        (bottomLeft.y + topRight.y) / 2.0
    );
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double factor) {
    Point oldCenter = getCenter();

    for (auto& shape : shapes_) {
        shape->scale(factor);
    }

    Point newCenter = getCenter();

    double dx = oldCenter.x - newCenter.x;
    double dy = oldCenter.y - newCenter.y;

    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

size_t CompositeShape::getShapeCount() const {
    return shapes_.size();
}

const Shape* CompositeShape::getShape(size_t index) const {
    if (index < shapes_.size()) {
        return shapes_[index].get();
    }
    return nullptr;
}
void CompositeShape::print(std::ostream& os) const {
    os << "[" << getName() << ", " << getCenter() << ", " << getArea() << ":\n";
    for (size_t i = 0; i < shapes_.size(); ++i) {
        const Shape* s = shapes_[i].get();
        os << "  " << s->getName() << ", " << s->getCenter() << ", " << s->getArea();
        if (i < shapes_.size() - 1) {
            os << ",";
        }
        os << "\n";
    }
    os << "]";
}
