#include "Rectangle.h"

Rectangle::Rectangle(Point left, Point right) : left_(left), right_(right) {}

double Rectangle::getArea() const { return (right_.y_ - left_.y_) * (right_.x_ - left_.x_); }

Point Rectangle::getCenter() const
{
    double xc = left_.x_ + (right_.x_ - left_.x_) / 2;
    double yc = left_.y_ + (right_.y_ - left_.y_) / 2;
    return Point(xc, yc);
}

void Rectangle::move(double dx, double dy)
{
    left_.x_ += dx;
    left_.y_ += dy;
    right_.x_ += dx;
    right_.y_ += dy;
}

void Rectangle::scale(double k)
{
    Point c = getCenter();
    left_.x_ = c.x_ + (left_.x_ - c.x_) * k;
    left_.y_ = c.y_ + (left_.y_ - c.y_) * k;
    right_.x_ = c.x_ + (right_.x_ - c.x_) * k;
    right_.y_ = c.y_ + (right_.y_ - c.y_) * k;
}

std::string Rectangle::getName() const { return "RECTANGLE"; }

Point Rectangle::getBottomLeft() const { return left_; }
Point Rectangle::getTopRight() const { return right_; }

void Rectangle::print() const
{
    std::cout << "[" << getName() << ", (" << getCenter().x_ << ", "
        << getCenter().y_ << "), " << getArea() << "]";
}
