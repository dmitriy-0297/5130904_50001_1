#include "rhombus.h"

Rhombus::Rhombus(Point center, double horizontalDiag, double verticalDiag):
  center_(center),
  horizontalDiag_(horizontalDiag),
  verticalDiag_(verticalDiag)
{}

double Rhombus::getArea() const {
  return 0.5 * horizontalDiag_ * verticalDiag_;
}

Point Rhombus::getCenter() const {
  return center_;
}

void Rhombus::move(double dx, double dy) {
  center_.x_ += dx;
  center_.y_ += dy;
}

void Rhombus::scale(double factor) {
  horizontalDiag_ *= factor;
  verticalDiag_ *= factor;
}

std::string Rhombus::getName() const {
  return "RHOMBUS";
}
