#include "point.h"
#include "rightTrapezium.h"
#include <string>
#include <stdexcept>

const std::string ERROR_COEFF_IS_LESS_THAN_OR_EQUAL_TO_0 =
"ERROR: Coefficient is less than or equal to 0";

const std::string ERROR_INCORRECT_BASES_SPECIFIED =
"ERROR: Incorrect bases specified";

RightTrapezium::RightTrapezium(const Point& leftDown,
    const double longBase, const double shortBase, const double height) :

    height_(height)
{

    if (!(longBase > shortBase)) {
        throw std::invalid_argument(ERROR_INCORRECT_BASES_SPECIFIED);
    }

    longBase_ = longBase;
    shortBase_ = shortBase;
    leftDown_.x = leftDown.x;
    leftDown_.y = leftDown.y;
}


double RightTrapezium::getArea() const {
    return (longBase_ + shortBase_) / 2 * height_;
}

Point RightTrapezium::getCenter() const {

    double midpointOfRightSideX = leftDown_.x + (longBase_ + shortBase_) / 2;

    Point center;

    center.x = (leftDown_.x + midpointOfRightSideX) / 2;
    center.y = leftDown_.y + height_ / 2;

    return center;

}

void RightTrapezium::move(const double dx, const double dy) {
    leftDown_.x += dx;
    leftDown_.y += dy;
}

void RightTrapezium::scale(const double coeff) {

    if (coeff <= 0.0) {
        throw std::invalid_argument(ERROR_COEFF_IS_LESS_THAN_OR_EQUAL_TO_0);
    }

    const Point CENTER = this->getCenter();

    leftDown_.x = CENTER.x - (CENTER.x - leftDown_.x) * coeff;
    leftDown_.y = CENTER.y - (CENTER.y - leftDown_.y) * coeff;

    longBase_ *= coeff;
    shortBase_ *= coeff;
    height_ *= coeff;

}

std::string RightTrapezium::getName() const {
    return "RIGHT TRAPEZIUM";
}

std::vector<Point> RightTrapezium::getPoints() const {

    Point leftUp;
    leftUp.x = leftDown_.x;
    leftUp.y = leftDown_.y + height_;

    Point rightDown;
    rightDown.x = leftDown_.x + longBase_;
    rightDown.y = leftDown_.y;

    Point rightUp;
    rightUp.x = leftDown_.x + shortBase_;
    rightUp.y = leftDown_.y + height_;

    return std::vector<Point>{leftDown_, leftUp, rightDown, rightUp};

}
