#include "rectangle.h"

Rectangle::Rectangle(Point bottomLeft, Point topRight):
  bottomLeft_(bottomLeft),
  topRight_(topRight)
{}

double Rectangle::getArea() const {
  return (topRight_.x_ - bottomLeft_.x_) * (topRight_.y_ - bottomLeft_.y_);
}

Point Rectangle::getCenter() const {
  double centerX = (bottomLeft_.x_ + topRight_.x_) / 2.0;
  double centerY = (bottomLeft_.y_ + topRight_.y_) / 2.0;
  return {centerX, centerY};
}

void Rectangle::move(double dx, double dy) {
  bottomLeft_.x_ += dx;
  bottomLeft_.y_ += dy;
  topRight_.x_ += dx;
  topRight_.y_ += dy;
}

void Rectangle::scale(double factor) {
  Point center = getCenter();
  bottomLeft_.x_ = center.x_ + (bottomLeft_.x_ - center.x_) * factor;
  bottomLeft_.y_ = center.y_ + (bottomLeft_.y_ - center.y_) * factor;
  topRight_.x_ = center.x_ + (topRight_.x_ - center.x_) * factor;
  topRight_.y_ = center.y_ + (topRight_.y_ - center.y_) * factor;
}

std::string Rectangle::getName() const {
  return "RECTANGLE";
}
