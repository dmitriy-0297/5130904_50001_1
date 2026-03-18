#ifndef RHOMBUS_H
#define RHOMBUS_H

#include "shape.h"

class Rhombus : public Shape {
public:
  Rhombus(Point center, double horizontalDiag, double verticalDiag);
  double getArea() const override;
  Point getCenter() const override;
  void move(double dx, double dy) override;
  void scale(double factor) override;
  std::string getName() const override;

private:
  Point center_;
  double horizontalDiag_;
  double verticalDiag_;
};

#endif
