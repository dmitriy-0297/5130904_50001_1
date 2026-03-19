#include "rectangle.hpp"
#include <cmath>

Rectangle::Rectangle(const Point &leftBottom, const Point &rightTop)
{

  leftBottom_.x = leftBottom.x;
  leftBottom_.y = leftBottom.y;
  rightTop_.x = rightTop.x;
  rightTop_.y = rightTop.y;
}

double Rectangle::getArea() const
{

  return (rightTop_.x - leftBottom_.x) * (rightTop_.y - leftBottom_.y);
}

Point Rectangle::getCenter() const
{

  return {
      (leftBottom_.x + rightTop_.x) / 2.0,
      (leftBottom_.y + rightTop_.y) / 2.0};
}

void Rectangle::move(double dx, double dy)
{

  leftBottom_.x += dx;
  leftBottom_.y += dy;
  rightTop_.x += dx;
  rightTop_.y += dy;
}

void Rectangle::scale(double coefficient)
{

  Point center = getCenter();
  double width = rightTop_.x - leftBottom_.x;
  double height = rightTop_.y - leftBottom_.y;

  double newWidth = width * coefficient;
  double newHeight = height * coefficient;

  leftBottom_.x = center.x - newWidth / 2.0;
  rightTop_.x = center.x + newWidth / 2.0;
  leftBottom_.y = center.y - newHeight / 2.0;
  rightTop_.y = center.y + newHeight / 2.0;
}

std::string Rectangle::getName() const
{

  return "RECTANGLE";
}

FrameRect Rectangle::getFrameRect() const
{

  Point center = getCenter();

  return {
      center.x,
      center.y,
      rightTop_.x - leftBottom_.x,
      rightTop_.y - leftBottom_.y};
}
