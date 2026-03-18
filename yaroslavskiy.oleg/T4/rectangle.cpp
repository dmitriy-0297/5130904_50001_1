
#include "rectangle.hpp"

Rectangle::Rectangle(const Point& t1, const Point& t2)
    : bottomLeft(t1), topRight(t2) {
}

double Rectangle::getArea() const {
    double width = topRight.x - bottomLeft.x;
    double height = topRight.y - bottomLeft.y;
    return width * height;
}

Point Rectangle::getCenter() const {
    double centerX = (bottomLeft.x + topRight.x) * 0.5;
    double centerY = (bottomLeft.y + topRight.y) * 0.5;
    return Point(centerX, centerY);
}

void Rectangle::move(double dx, double dy) {
    bottomLeft.x += dx;
    bottomLeft.y += dy;
    topRight.x += dx;
    topRight.y += dy;
}

void Rectangle::scale(double coef) {

    Point center = getCenter();

    bottomLeft.x = center.x + (bottomLeft.x - center.x) * coef;
    bottomLeft.y = center.y + (bottomLeft.y - center.y) * coef;
    topRight.x = center.x + (topRight.x - center.x) * coef;
    topRight.y = center.y + (topRight.y - center.y) * coef;
}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}




