#include "rectangle.h"

Rectangle::Rectangle(Point lb, Point rt)
    : leftBottom_(lb), rightTop_(rt) {}

double Rectangle::getArea() const
{
    return (rightTop_.x - leftBottom_.x) *
           (rightTop_.y - leftBottom_.y);
}

Point Rectangle::getCenter() const
{
    return {
        (leftBottom_.x + rightTop_.x) / 2,
        (leftBottom_.y + rightTop_.y) / 2
    };
}

void Rectangle::move(double dx, double dy)
{
    leftBottom_.x += dx;
    leftBottom_.y += dy;
    rightTop_.x += dx;
    rightTop_.y += dy;
}

void Rectangle::scale(double k)
{
    Point c = getCenter();

    leftBottom_.x = c.x + (leftBottom_.x - c.x) * k;
    leftBottom_.y = c.y + (leftBottom_.y - c.y) * k;

    rightTop_.x = c.x + (rightTop_.x - c.x) * k;
    rightTop_.y = c.y + (rightTop_.y - c.y) * k;
}

std::string Rectangle::getName() const
{
    return "RECTANGLE";
}
