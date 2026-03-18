#ifndef SHAPE_H
#define SHAPE_H

#include <iostream>
#include <string>
#include "Point.h"

class Shape {
private:
  std::string name_ = "";

public:
  Shape() = default;
  Shape(const std::string& name) : name_(name) {}
  Shape(const Shape&) = delete;
  Shape& operator=(const Shape&) = delete;
  Shape(Shape&&) = default;
  Shape& operator=(Shape&&) = default;
  virtual ~Shape() = default;

  std::string getName() const { return name_; }
  virtual double getArea() const = 0;
  virtual Point getCenter() const = 0;

  virtual void move(double dx, double dy) = 0;
  virtual void scale(double factor) = 0;

  virtual void print(std::ostream& os) const {
    os << "[" << getName() << ", " << getCenter() << ", " << getArea() << "]";
  }
};

inline std::ostream& operator<<(std::ostream& os, const Shape& shape) {
  shape.print(os);
  return os;
}
#endif
