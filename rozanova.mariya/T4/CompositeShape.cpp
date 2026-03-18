#include "CompositeShape.h"
#include <algorithm>
#include <limits>
#include <iostream>

void CompositeShape::addShape(std::unique_ptr<Shape> shape) {
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const
{
    double area = 0.0;
    for (const auto& shape : shapes_) {
        area += shape->getArea();
    }
    return area;
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double k) {
    if (shapes_.empty()) return;

    Point center = getCenter();

    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();
        double dx = shapeCenter.x_ - center.x_;
        double dy = shapeCenter.y_ - center.y_;

        shape->move(-dx, -dy);
        shape->scale(k);
        shape->move(dx * k, dy * k);
    }
}

std::string CompositeShape::getName() const { return "COMPOSITE"; }

Point CompositeShape::getBottomLeft() const {
    if (shapes_.empty()) return Point(0, 0);

    double minX = shapes_[0]->getBottomLeft().x_;
    double minY = shapes_[0]->getBottomLeft().y_;

    for (const auto& shape : shapes_) {
        Point bl = shape->getBottomLeft();
        minX = std::min(minX, bl.x_);
        minY = std::min(minY, bl.y_);
    }

    return Point(minX, minY);
}

Point CompositeShape::getTopRight() const {
    if (shapes_.empty()) return Point(0, 0);

    double maxX = shapes_[0]->getTopRight().x_;
    double maxY = shapes_[0]->getTopRight().y_;

    for (const auto& shape : shapes_) {
        Point tr = shape->getTopRight();
        maxX = std::max(maxX, tr.x_);
        maxY = std::max(maxY, tr.y_);
    }

    return Point(maxX, maxY);
}

Point CompositeShape::getCenter() const {
    if (shapes_.empty()) {
        return Point(0, 0);
    }

    Point bl = getBottomLeft();
    Point tr = getTopRight();

    return Point((bl.x_ + tr.x_) / 2, (bl.y_ + tr.y_) / 2);
}

void CompositeShape::print() const
{
    std::cout << "[" << getName() << ", (" << getCenter().x_ << ", " << getCenter().y_ << "), " << getArea() << ":\n";

    for (size_t i = 0; i < shapes_.size(); ++i) {
        std::cout << "  " << shapes_[i]->getName() << ", (" << shapes_[i]->getCenter().x_ << ", "
            << shapes_[i]->getCenter().y_ << "), " << shapes_[i]->getArea();
        if (i < shapes_.size() - 1)
            std::cout << ",";
        std::cout << "\n";
    }
    std::cout << "]";
}
