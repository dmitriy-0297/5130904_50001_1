#include "point.h"
#include "rectangle.h"
#include <string>
#include <stdexcept>
#include <vector>

const std::string ERROR_COEFF_IS_LESS_THAN_OR_EQUAL_TO_0 =
    "ERROR: Coefficient is less than or equal to 0";

const std::string ERROR_INCORRECT_POINTS_SPECIFIED =
    "ERROR: Incorrect points specified";

Rectangle::Rectangle(const Point& leftDown, const Point& rightUp) {

    if (!(leftDown.x < rightUp.x && leftDown.y < rightUp.y)) {
        throw std::invalid_argument(ERROR_INCORRECT_POINTS_SPECIFIED);
    }

    leftDown_.x = leftDown.x;
    leftDown_.y = leftDown.y;
    rightUp_.x = rightUp.x;
    rightUp_.y = rightUp.y;
}

double Rectangle::getArea() const {
    return (rightUp_.x - leftDown_.x) * (rightUp_.y - leftDown_.y);
}

Point Rectangle::getCenter() const {

    Point center;

    center.x = (leftDown_.x + rightUp_.x) / 2;
    center.y = (leftDown_.y + rightUp_.y) / 2;

    return center;
}

void Rectangle::move(const double dx, const double dy) {
    leftDown_.x += dx;
    leftDown_.y += dy;
    rightUp_.x += dx;
    rightUp_.y += dy;
}

void Rectangle::scale(const double coeff) {

    if (coeff <= 0.0) {
        throw std::invalid_argument(ERROR_COEFF_IS_LESS_THAN_OR_EQUAL_TO_0);
    }

    const Point CENTER = this->getCenter();

    leftDown_.x = CENTER.x - (CENTER.x - leftDown_.x) * coeff;
    leftDown_.y = CENTER.y - (CENTER.y - leftDown_.y) * coeff;

    rightUp_.x = CENTER.x + (rightUp_.x - CENTER.x) * coeff;
    rightUp_.y = CENTER.y + (rightUp_.y - CENTER.y) * coeff;

}

std::string Rectangle::getName() const {
    return "RECTANGLE";
}

std::vector<Point> Rectangle::getPoints() const {

    Point leftUp;
    leftUp.x = leftDown_.x;
    leftUp.y = rightUp_.y;

    Point rightDown;
    rightDown.x = rightUp_.x;
    rightDown.y = leftDown_.y;

    return std::vector<Point>{leftDown_, leftUp, rightDown, rightUp_};

}
