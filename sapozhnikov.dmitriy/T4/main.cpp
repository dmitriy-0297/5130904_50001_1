#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "rectangle.hpp"
#include "right_trapezoid.hpp"
#include "composite_shape.hpp"

void printShapeInfo(const std::shared_ptr<Shape> &shape)
{
  std::cout << std::fixed << std::setprecision(2);

  Point center = shape->getCenter();
  std::string name = shape->getName();

  auto composite = std::dynamic_pointer_cast<CompositeShape>(shape);

  if (composite)
  {
    std::cout << "[" << name << ", (" << center.x << ", " << center.y << "), " << shape->getArea() << ":\n";
    for (size_t i = 0; i < composite->getSize(); ++i)
    {
      auto child = composite->getShapeAtIndex(i);
      Point childCenter = child->getCenter();
      std::cout << "  " << child->getName() << ", ("
                << childCenter.x << ", " << childCenter.y << "), "
                << child->getArea();
      if (i < composite->getSize() - 1)
        std::cout << ",\n";
      else
        std::cout << "\n";
    }
    std::cout << "]" << std::endl;
  }
  else
  {
    std::cout << "[" << name << ", (" << center.x << ", " << center.y << "), " << shape->getArea() << "]" << std::endl;
  }
}

int main()
{

  std::vector<std::shared_ptr<Shape>> shapes;

  shapes.push_back(std::make_shared<Rectangle>(Point{0.0, 0.0}, Point{4.0, 2.0}));

  shapes.push_back(std::make_shared<RightTrapezoid>(Point{0.0, 0.0}, 4.0, 2.0, 2.0));

  shapes.push_back(std::make_shared<Rectangle>(Point{10.0, 10.0}, Point{12.0, 12.0}));

  shapes.push_back(std::make_shared<RightTrapezoid>(Point{5.0, 5.0}, 6.0, 2.0, 4.0));

  auto composite = std::make_shared<CompositeShape>();

  composite->addShape(std::make_shared<Rectangle>(Point{0.0, 0.0}, Point{2.0, 2.0}));
  composite->addShape(std::make_shared<RightTrapezoid>(Point{2.0, 0.0}, 2.0, 2.0, 2.0));
  shapes.push_back(composite);

  std::cout << "   BEFORE SCALING   " << std::endl;
  for (const auto &shape : shapes)
  {
    printShapeInfo(shape);
  }

  for (const auto &shape : shapes)
  {
    shape->scale(2.0);
  }

  std::cout << "\n   AFTER SCALING (x2)   " << std::endl;
  for (const auto &shape : shapes)
  {
    printShapeInfo(shape);
  }

  return 0;
}
