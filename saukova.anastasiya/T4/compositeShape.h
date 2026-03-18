#ifndef COMPOSITESHAPE_H
#define COMPOSITESHAPE_H

#include <vector>
#include "shape.h"

class CompositeShape : public Shape {
public:
  CompositeShape();
  ~CompositeShape() override;

  void addShape(Shape* shape);
  double getArea() const override;
  Point getCenter() const override;
  void move(double dx, double dy) override;
  void scale(double factor) override;
  std::string getName() const override;

private:
  std::vector<Shape*> shapes_;
};

#endif
