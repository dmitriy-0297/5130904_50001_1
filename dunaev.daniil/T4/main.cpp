#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>

#include "rectangle.h"
#include "square.h"
#include "compositeShape.h"

void printShape(const Shape& s)
{
    Point c = s.getCenter();

    std::cout << "["
              << s.getName()
              << ", (" << c.x << ", " << c.y << "), "
              << s.getArea()
              << "]\n";
}

int main()
{
    std::cout << std::fixed << std::setprecision(2);

    std::vector<std::unique_ptr<Shape>> shapes;

    shapes.push_back(std::make_unique<Rectangle>(Point{0,0}, Point{4,3}));
    shapes.push_back(std::make_unique<Square>(Point{1,1}, 2));
    shapes.push_back(std::make_unique<Square>(Point{3,3}, 1));
    shapes.push_back(std::make_unique<Square>(Point{5,5}, 2));

    auto comp = std::make_unique<CompositeShape>();

    comp->addShape(std::make_unique<Square>(Point{0,0},2));
    comp->addShape(std::make_unique<Rectangle>(Point{1,1},Point{3,2}));

    shapes.push_back(std::move(comp));

    std::cout << "Before scale:\n";

    for (const auto& s : shapes)
        printShape(*s);

    for (auto& s : shapes)
        s->scale(2);

    std::cout << "\nAfter scale:\n";

    for (const auto& s : shapes)
        printShape(*s);
}
