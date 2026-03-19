#ifndef RIGHT_TRAPEZOID_HPP
#define RIGHT_TRAPEZOID_HPP

#include "shape.hpp"

class RightTrapezoid : public Shape
{

public:
  RightTrapezoid(const Point &leftBottom, double bottomBase, double topBase, double height);

  double getArea() const override;
  Point getCenter() const override;
  void move(double dx, double dy) override;
  void scale(double coefficient) override;
  std::string getName() const override;
  FrameRect getFrameRect() const override;

private:
  Point leftBottom_;
  double bottomBase_;
  double topBase_;
  double height_;
};

#endif
