#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include "point.h"
#include "shape.h"
#include "rectangle.h"
#include "square.h"
#include "composite_shape.h"

int main()
{
    std::vector<std::shared_ptr<Shape>> figures;

    figures.push_back(std::make_shared<Rectangle>(Point(0.0, 0.0), Point(4.0, 3.0)));
    figures.push_back(std::make_shared<Rectangle>(Point(5.0, 5.0), Point(10.0, 8.0)));
    figures.push_back(std::make_shared<Square>(Point(1.0, 1.0), 5.0));
    figures.push_back(std::make_shared<Square>(Point(10.0, 0.0), 2.0));

    std::shared_ptr<CompositeShape> composite = std::make_shared<CompositeShape>();
    composite->addShape(std::make_shared<Rectangle>(Point(0.0, 0.0), Point(2.0, 2.0)));
    composite->addShape(std::make_shared<Square>(Point(2.0, 0.0), 2.0));
    figures.push_back(composite);

    std::cout << std::fixed << std::setprecision(2);
    for (const auto& fig : figures) {
        fig->print(std::cout);
        std::cout << "\n";
    }

    for (auto& fig : figures) {
        fig->scale(2.0);
    }

    for (const auto& fig : figures) {
        fig->print(std::cout);
        std::cout << "\n";
    }

    return 0;
}
