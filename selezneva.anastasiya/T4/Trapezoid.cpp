#include "Trapezoid.h"
#include <cmath>
#include <stdexcept>
#include <string>

const std::string  INVALID_ARGUMENT_ERROR = "All dimensions must be positive";

Trapezoid::Trapezoid(const Point& bottomLeft, double bottomBase, double topBase, double height)
    : Shape("TRAPEZOID"), bottomLeft_(bottomLeft),
    bottomBase_(bottomBase), topBase_(topBase), height_(height) {

    if (bottomBase_ <= 0 || topBase_ <= 0 || height_ <= 0) {
        throw std::invalid_argument(INVALID_ARGUMENT_ERROR);
    }
}

double Trapezoid::getArea() const {
    return (bottomBase_ + topBase_) / 2.0 * height_;
}

Point Trapezoid::getCenter() const {
    double centerX = bottomLeft_.x + (bottomBase_ + topBase_) / 4.0;
    double centerY = bottomLeft_.y + height_ / 2.0;
    return Point(centerX, centerY);
}

void Trapezoid::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
}

void Trapezoid::scale(double factor) {
    Point oldCenter = getCenter();

    bottomBase_ *= factor;
    topBase_ *= factor;
    height_ *= factor;

    Point newCenter = getCenter();

    bottomLeft_.x += oldCenter.x - newCenter.x;
    bottomLeft_.y += oldCenter.y - newCenter.y;
}

Point Trapezoid::getBottomLeft() const {
    return bottomLeft_;
}

Point Trapezoid::getBottomRight() const {
    return Point(bottomLeft_.x + bottomBase_, bottomLeft_.y);
}

Point Trapezoid::getTopLeft() const {
    double offset = (bottomBase_ - topBase_) / 2.0;
    return Point(bottomLeft_.x + offset, bottomLeft_.y + height_);
}

Point Trapezoid::getTopRight() const {
    double offset = (bottomBase_ - topBase_) / 2.0;
    return Point(bottomLeft_.x + bottomBase_ - offset, bottomLeft_.y + height_);
}
