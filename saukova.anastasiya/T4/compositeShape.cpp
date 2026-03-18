#include "compositeShape.h"
#include <stdexcept>

CompositeShape::CompositeShape():
  shapes_()
{}

CompositeShape::~CompositeShape() {
  for (Shape* shape : shapes_) {
    delete shape;
  }
}

void CompositeShape::addShape(Shape* shape) {
  if (shape != nullptr) {
    shapes_.push_back(shape);
  }
}

double CompositeShape::getArea() const {
  double totalArea = 0.0;
  for (const Shape* shape : shapes_) {
    totalArea += shape->getArea();
  }
  return totalArea;
}

Point CompositeShape::getCenter() const {
  if (shapes_.empty()) {
    return {0.0, 0.0};
  }

  double sumX = 0.0;
  double sumY = 0.0;
  for (const Shape* shape : shapes_) {
    Point currentCenter = shape->getCenter();
    sumX += currentCenter.x_;
    sumY += currentCenter.y_;
  }

  double count = static_cast<double>(shapes_.size());
  return {sumX / count, sumY / count};
}

void CompositeShape::move(double dx, double dy) {
  for (Shape* shape : shapes_) {
    shape->move(dx, dy);
  }
}

void CompositeShape::scale(double factor) {
  if (factor <= 0.0) {
    return;
  }

  Point commonCenter = getCenter();
  for (Shape* shape : shapes_) {
    Point oldCenter = shape->getCenter();

    double offsetX = (oldCenter.x_ - commonCenter.x_) * (factor - 1.0);
    double offsetY = (oldCenter.y_ - commonCenter.y_) * (factor - 1.0);

    shape->move(offsetX, offsetY);
    shape->scale(factor);
  }
}

std::string CompositeShape::getName() const {
  return "COMPOSITE";
}
