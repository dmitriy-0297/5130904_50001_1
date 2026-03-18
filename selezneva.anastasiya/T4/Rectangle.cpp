#include "Rectangle.h"
#include <cmath>

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight)
    : Shape("RECTANGLE"), bottomLeft_(bottomLeft), topRight_(topRight) {
}

double Rectangle::getArea() const {
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;
    return std::abs(width * height);
}

Point Rectangle::getCenter() const {
    return Point(
        (bottomLeft_.x + topRight_.x) / 2.0,
        (bottomLeft_.y + topRight_.y) / 2.0
    );
}

void Rectangle::move(double dx, double dy) {
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double factor) {
    Point center = getCenter();
    bottomLeft_.x = center.x - (center.x - bottomLeft_.x) * factor;
    bottomLeft_.y = center.y - (center.y - bottomLeft_.y) * factor;
    topRight_.x = center.x + (topRight_.x - center.x) * factor;
    topRight_.y = center.y + (topRight_.y - center.y) * factor;
}

Point Rectangle::getBottomLeft() const {
    return bottomLeft_;
}

Point Rectangle::getTopRight() const {
    return topRight_;
}
