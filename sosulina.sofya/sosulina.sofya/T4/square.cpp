#include "square.h"

Square::Square(const Point& bottomLeft, double side) :
    bottomLeft_(bottomLeft),
    side_(side)
{
}

Square::~Square()
{
}

double Square::getArea() const
{
    return side_ * side_;
}

Point Square::getCenter() const
{
    double centerX = bottomLeft_.x_ + side_ / 2.0;
    double centerY = bottomLeft_.y_ + side_ / 2.0;
    return Point(centerX, centerY);
}

void Square::move(const Point& delta)
{
    bottomLeft_.x_ += delta.x_;
    bottomLeft_.y_ += delta.y_;
}

void Square::scale(double factor)
{
    Point center = getCenter();
    side_ *= factor;
    bottomLeft_.x_ = center.x_ - side_ / 2.0;
    bottomLeft_.y_ = center.y_ - side_ / 2.0;
}

std::string Square::getName() const
{
    return "SQUARE";
}

std::pair<Point, Point> Square::getBoundingBox() const
{
    Point topRight(bottomLeft_.x_ + side_, bottomLeft_.y_ + side_);
    return std::make_pair(bottomLeft_, topRight);
}

void Square::print(std::ostream& os) const
{
    Point center = getCenter();
    double area = getArea();
    os << "[" << getName() << ", (";
    os << center.x_ << ", " << center.y_ << "), ";
    os << area << "]";
}
