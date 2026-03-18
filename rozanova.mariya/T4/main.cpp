#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include "Rectangle.h"
#include "Ellipse.h"
#include "CompositeShape.h"

int main() {
    std::cout << std::fixed << std::setprecision(2);

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point(0.0, 0.0), Point(4.0, 3.0)));
    shapes.push_back(std::make_unique<Rectangle>(Point(-1.0, -2.0), Point(2.0, 4.0)));
    shapes.push_back(std::make_unique<Ellipse>(Point(5.0, 5.0), 2.0, 1.5));
    shapes.push_back(std::make_unique<Ellipse>(Point(-3.0, 4.0), 1.0, 0.8));

    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(Point(10.0, 10.0), Point(14.0, 13.0)));
    composite->addShape(std::make_unique<Rectangle>(Point(15.0, 8.0), Point(18.0, 12.0)));
    composite->addShape(std::make_unique<Ellipse>(Point(12.0, 11.0), 1.5, 2.0));
    shapes.push_back(std::move(composite));

    std::cout << "BEFORE SCALING:\n\n";
    for (const auto& shape : shapes) { shape->print(); std::cout << "\n\n"; }

    for (auto& shape : shapes) { shape->scale(2.0); }

    std::cout << "AFTER SCALING (x2):\n\n";
    for (const auto& shape : shapes) { shape->print(); std::cout << "\n\n"; }

    return EXIT_SUCCESS;
}
