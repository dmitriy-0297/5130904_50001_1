#include "right_trapezoid.hpp"

RightTrapezoid::RightTrapezoid(const Point &leftBottom, double bottomBase, double topBase, double height)
{

  leftBottom_ = leftBottom;
  bottomBase_ = bottomBase;
  topBase_ = topBase;
  height_ = height;
}

double RightTrapezoid::getArea() const
{

  return (bottomBase_ + topBase_) / 2.0 * height_;
}

Point RightTrapezoid::getCenter() const
{

  return {

      leftBottom_.x + (bottomBase_ + topBase_) / 4.0,
      leftBottom_.y + height_ / 2.0

  };
}

void RightTrapezoid::move(double dx, double dy)
{

  leftBottom_.x += dx;
  leftBottom_.y += dy;
}

void RightTrapezoid::scale(double coefficient)
{
  Point center = getCenter();
  bottomBase_ *= coefficient;
  topBase_ *= coefficient;
  height_ *= coefficient;
  leftBottom_.x = center.x - (bottomBase_ + topBase_) / 4.0;
  leftBottom_.y = center.y - height_ / 2.0;
}

std::string RightTrapezoid::getName() const
{
  return "RIGHT_TRAPEZOID";
}

FrameRect RightTrapezoid::getFrameRect() const
{
  double maxWidth = std::max(bottomBase_, topBase_);
  double centerX = leftBottom_.x + maxWidth / 2.0;
  double centerY = leftBottom_.y + height_ / 2.0;
  return {centerX, centerY, maxWidth, height_};
}
