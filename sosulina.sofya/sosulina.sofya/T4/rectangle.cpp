#include "rectangle.h"

Rectangle::Rectangle(const Point& bottomLeft, const Point& topRight) :
    bottomLeft_(bottomLeft),
    topRight_(topRight)
{
}

Rectangle::~Rectangle()
{
}

double Rectangle::getArea() const
{
    double width = topRight_.x_ - bottomLeft_.x_;
    double height = topRight_.y_ - bottomLeft_.y_;
    return width * height;
}

Point Rectangle::getCenter() const
{
    double centerX = (bottomLeft_.x_ + topRight_.x_) / 2.0;
    double centerY = (bottomLeft_.y_ + topRight_.y_) / 2.0;
    return Point(centerX, centerY);
}

void Rectangle::move(const Point& delta)
{
    bottomLeft_.x_ += delta.x_;
    bottomLeft_.y_ += delta.y_;
    topRight_.x_ += delta.x_;
    topRight_.y_ += delta.y_;
}

void Rectangle::scale(double factor)
{
    Point center = getCenter();
    double width = topRight_.x_ - bottomLeft_.x_;
    double height = topRight_.y_ - bottomLeft_.y_;

    width *= factor;
    height *= factor;

    bottomLeft_.x_ = center.x_ - width / 2.0;
    bottomLeft_.y_ = center.y_ - height / 2.0;
    topRight_.x_ = center.x_ + width / 2.0;
    topRight_.y_ = center.y_ + height / 2.0;
}

std::string Rectangle::getName() const
{
    return "RECTANGLE";
}

std::pair<Point, Point> Rectangle::getBoundingBox() const
{
    return std::make_pair(bottomLeft_, topRight_);
}

void Rectangle::print(std::ostream& os) const
{
    Point center = getCenter();
    double area = getArea();
    os << "[" << getName() << ", (";
    os << center.x_ << ", " << center.y_ << "), ";
    os << area << "]";
}
