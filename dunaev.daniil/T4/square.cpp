#include "square.h"

Square::Square(Point lb, double side)
    : leftBottom_(lb), side_(side) {}

double Square::getArea() const
{
    return side_ * side_;
}

Point Square::getCenter() const
{
    return {
        leftBottom_.x + side_ / 2,
        leftBottom_.y + side_ / 2
    };
}

void Square::move(double dx, double dy)
{
    leftBottom_.x += dx;
    leftBottom_.y += dy;
}

void Square::scale(double k)
{
    side_ *= k;
}

std::string Square::getName() const
{
    return "SQUARE";
}
