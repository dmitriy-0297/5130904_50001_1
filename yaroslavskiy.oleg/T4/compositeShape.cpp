
#include "compositeShape.hpp"
#include <limits>
#include <algorithm>
#include <iostream>

CompositeShape::CompositeShape() {}

CompositeShape::~CompositeShape() {
    for (Shape* shape : shapes) {
        delete shape;
    }
}

void CompositeShape::addShape(Shape* shape) {
    shapes.push_back(shape);
}

double CompositeShape::getArea() const {
    double total = 0;
    for (Shape* shape : shapes) {
        total += shape->getArea();
    }
    return total;
}

Point CompositeShape::getCenter() const {
    if (shapes.empty()) {
        return Point(0, 0);
    }

    double minX = std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max();
    double maxX = -std::numeric_limits<double>::max();
    double maxY = -std::numeric_limits<double>::max();

    for (Shape* shape : shapes) {
        Point center = shape->getCenter();
        minX = std::min(minX, center.x);
        minY = std::min(minY, center.y);
        maxX = std::max(maxX, center.x);
        maxY = std::max(maxY, center.y);
    }

    return Point((minX + maxX) / 2, (minY + maxY) / 2);
}

void CompositeShape::move(double dx, double dy) {
    for (Shape* shape : shapes) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double coef) {
    Point center = getCenter();

    for (Shape* shape : shapes) {

        Point shapeCenter = shape->getCenter();
        double dx = shapeCenter.x - center.x;
        double dy = shapeCenter.y - center.y;

        shape->move(dx * (coef - 1), dy * (coef - 1));
        shape->scale(coef);
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::printDetails() const {
    std::cout << "[" << getName() << ", ("
        << getCenter().x << ", " << getCenter().y << "), "
        << getArea() << ":" << std::endl;

    for (Shape* shape : shapes) {
        Point center = shape->getCenter();
        std::cout << "  " << shape->getName() << ", ("
            << center.x << ", " << center.y << "), "
            << shape->getArea() << std::endl;
    }
    std::cout << "]" << std::endl;
}

