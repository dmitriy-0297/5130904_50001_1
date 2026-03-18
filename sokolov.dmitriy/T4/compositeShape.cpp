#include "point.h"
#include "compositeShape.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

const std::string ERROR_COEFF_IS_LESS_THAN_OR_EQUAL_TO_0 =
"ERROR: Coefficient is less than or equal to 0";

const std::string ERROR_INCORRECT_INDEX = "ERROR: Incorrect index";

const std::string ERROR_COMPOSITE_SHAPE_IS_EMPTY = "ERROR: Composite shape is empty";

CompositeShape::CompositeShape() {}

void CompositeShape::addShape(std::unique_ptr<Shape> newShape) {
    if (newShape) {
        shapes_.push_back(std::move(newShape));
    }
}

void CompositeShape::removeShape(const size_t index) {
    if (index >= shapes_.size()) {
        throw std::out_of_range(ERROR_INCORRECT_INDEX);
    }
    shapes_.erase(shapes_.begin() + index);
}

const Shape* CompositeShape::operator[](const size_t n) const {
    return shapes_[n].get();
}

size_t CompositeShape::size() const {
    return shapes_.size();
}

double CompositeShape::getArea() const {

    double area = 0.0;

    for (auto& shape : shapes_) {
        if (shape) {
            area += shape->getArea();
        }
    }

    return area;
}

Point CompositeShape::getCenter() const {

    Point leftDown;
    leftDown.x = 0.0;
    leftDown.y = 0.0;

    Point rightUp;
    rightUp.x = 0.0;
    rightUp.y = 0.0;

    bool centerSet = false;

    for (auto& shape : shapes_) {
        if (shape) {

            for (auto& point : shape->getPoints()) {

                if (point.x < leftDown.x || !centerSet) {
                    leftDown.x = point.x;
                }
                if (point.x > rightUp.x || !centerSet) {
                    rightUp.x = point.x;
                }
                if (point.y < leftDown.y || !centerSet) {
                    leftDown.y = point.y;
                }
                if (point.y > rightUp.y || !centerSet) {
                    rightUp.y = point.y;
                }

                centerSet = true;
            }
        }
    }

    Point center;
    center.x = (leftDown.x + rightUp.x) / 2;
    center.y = (leftDown.y + rightUp.y) / 2;

    return center;
}

void CompositeShape::move(const double dx, const double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(const double coeff) {

    if (shapes_.empty()) {
        throw std::logic_error(ERROR_COMPOSITE_SHAPE_IS_EMPTY);
    }

    if (coeff <= 0.0) {
        throw std::invalid_argument(ERROR_COEFF_IS_LESS_THAN_OR_EQUAL_TO_0);
    }

    const Point COMPOSITE_CENTER = this->getCenter();

    for (auto& shape : shapes_) {

        if (shape) {

            Point shapeCenter = shape->getCenter();

            shape->scale(coeff);

            double dx = (shapeCenter.x - COMPOSITE_CENTER.x) * (coeff - 1.0);
            double dy = (shapeCenter.y - COMPOSITE_CENTER.y) * (coeff - 1.0);

            shape->move(dx, dy);
        }
    }
}

std::string CompositeShape::getName() const {
    return "COMPOSITE";
}

std::vector<Point> CompositeShape::getPoints() const {

    std::vector<Point> points{};

    for (auto& shape : shapes_) {
        if (shape) {
            for (auto& point : shape->getPoints()) {
                points.push_back(point);
            }
        }
    }

    return points;
}
