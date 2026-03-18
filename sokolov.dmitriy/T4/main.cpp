#include <iostream>
#include <iomanip>
#include <vector>
#include "point.h"
#include "rectangle.h"
#include "rightTrapezium.h"
#include "compositeShape.h"

void printShapes(const std::vector<std::unique_ptr<Shape>>&);

int main() {

    std::vector<std::unique_ptr<Shape>> shapes{};

    Point o;
    o.x = 0.0;
    o.y = 0.0;

    Point a;
    a.x = 10.0;
    a.y = 15.0;

    shapes.push_back(std::make_unique<Rectangle>(o, a));

    Point b;
    b.x = 5.0;
    b.y = 0.0;

    shapes.push_back(std::make_unique<Rectangle>(b, a));

    Point c;
    c.x = 8.0;
    c.y = 12.0;

    shapes.push_back(std::make_unique<RightTrapezium>(c, 16.0, 10.0, 6.0));

    Point d;
    d.x = 8.0;
    d.y = -5.0;

    shapes.push_back(std::make_unique<RightTrapezium>(d, 30.0, 10.0, 2.0));

    Point e;
    e.x = 16.0;
    e.y = 16.0;

    {
        std::unique_ptr<CompositeShape> composite = std::make_unique<CompositeShape>();

        composite->addShape(std::make_unique<Rectangle>(d, a));

        composite->addShape(std::make_unique<Rectangle>(d, e));

        composite->addShape(std::make_unique<RightTrapezium>(e, 2.0, 1.0, 1.0));

        shapes.push_back(std::move(composite));
    }

    printShapes(shapes);

    for (auto& shape : shapes) {
        shape->scale(2);
    }

    printShapes(shapes);

    return EXIT_SUCCESS;
}

void printShapes(const std::vector<std::unique_ptr<Shape>>& shapes) {

    for (auto& shape : shapes) {

        std::string name = shape->getName();
        Point center = shape->getCenter();
        double area = shape->getArea();

        if (name != "COMPOSITE") {
            std::cout << std::fixed << std::setprecision(2) <<
                '[' << name << ", (" << center.x << ", " << center.y << "), " << area << "]\n";
        }
        else {
            std::cout << std::fixed << std::setprecision(2) <<
                '[' << name << ", (" << center.x << ", " << center.y << "), " << area << ":\n";

            const CompositeShape* COMPOSITE_PTR = dynamic_cast<CompositeShape*>(shape.get());

            for (size_t i = 0; i < COMPOSITE_PTR->size(); ++i) {

                const Shape* SUB_SHAPE = (*COMPOSITE_PTR)[i];

                std::string subName = SUB_SHAPE->getName();
                Point subCenter = SUB_SHAPE->getCenter();
                double subArea = SUB_SHAPE->getArea();

                std::cout << std::fixed << std::setprecision(2) <<
                    "  " << subName << ", (" << subCenter.x << ", "
                    << subCenter.y << "), " << subArea << ",\n";
            }

            std::cout << "]\n";
        }
    }
}
