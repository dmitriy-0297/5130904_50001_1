#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Shape.h"

class Rectangle: public Shape {
private:
  Point bottomLeft_ = Point(0.0, 0.0);
  Point topRight_ = Point(1.0, 1.0);
public:
  Rectangle(const Point& bottomLeft, const Point& topRight);
  ~Rectangle() override = default;
  Rectangle(const Rectangle& other) = default;
  Rectangle& operator=(const Rectangle& other) = default;
  Rectangle(Rectangle&& other) = default;
  Rectangle& operator=(Rectangle&& other) = default;

  double getArea() const override;
  Point getCenter() const override;
  void move(double dx, double dy) override;
  void scale(double factor) override;

  Point getBottomLeft() const;
  Point getTopRight() const;

};
#endif
