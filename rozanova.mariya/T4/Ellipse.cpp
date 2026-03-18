#include "Ellipse.h"
#include <iostream>

const double PI = 3.14159265358979323846;

Ellipse::Ellipse(Point center, double rx, double ry) : center_(center), rx_(rx), ry_(ry) {}

double Ellipse::getArea() const { return PI * rx_ * ry_; }

Point Ellipse::getCenter() const { return center_; }

void Ellipse::move(double dx, double dy) {
    center_.x_ += dx;
    center_.y_ += dy;
}

void Ellipse::scale(double k) {
    rx_ *= k;
    ry_ *= k;
}

std::string Ellipse::getName() const { return "ELLIPSE"; }

Point Ellipse::getBottomLeft() const { return Point(center_.x_ - rx_, center_.y_ - ry_); }

Point Ellipse::getTopRight() const { return Point(center_.x_ + rx_, center_.y_ + ry_); }

void Ellipse::print() const
{
    std::cout << "[" << getName() << ", (" << getCenter().x_
        << ", " << getCenter().y_ << "), " << getArea() << "]";
}
