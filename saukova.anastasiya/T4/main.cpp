#include <iostream>
#include <vector>
#include <iomanip>
#include "rectangle.h"
#include "rhombus.h"
#include "compositeShape.h"

const double SCALE_FACTOR = 2.0;

void printShapeInfo(const Shape* shape) {
  if (shape == nullptr) {
    return;
  }

  Point center = shape->getCenter();
  std::cout << "[" << shape->getName() << ", ("
            << std::fixed << std::setprecision(2)
            << center.x_ << ", " << center.y_ << "), "
            << shape->getArea() << "]" << std::endl;
}

int main() {
  std::vector<Shape*> container;

  container.push_back(new Rectangle({0.0, 0.0}, {2.0, 2.0}));
  container.push_back(new Rhombus({5.0, 5.0}, 4.0, 2.0));
  container.push_back(new Rectangle({10.0, 10.0}, {15.0, 12.0}));
  container.push_back(new Rhombus({-2.0, -2.0}, 2.0, 2.0));

  CompositeShape* composite = new CompositeShape();
  composite->addShape(new Rectangle({1.0, 1.0}, {2.0, 2.0}));
  composite->addShape(new Rhombus({1.5, 1.5}, 0.5, 0.5));
  container.push_back(composite);

  std::cout << "--- Before Scaling ---" << std::endl;
  for (const Shape* s : container) {
    printShapeInfo(s);
  }

  std::cout << "\n--- After Scaling (x2.0) ---" << std::endl;
  for (Shape* s : container) {
    s->scale(SCALE_FACTOR);
    printShapeInfo(s);
  }

  for (Shape* s : container) {
    delete s;
  }

  return 0;
}
