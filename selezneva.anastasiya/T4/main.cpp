#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include "Rectangle.h"
#include "Trapezoid.h"
#include "CompositeShape.h"

std::vector<std::unique_ptr<Shape>> createShapes();
void printShapes(const std::vector<std::unique_ptr<Shape>>& shapes);
void scaleAllShapes(std::vector<std::unique_ptr<Shape>>& shapes, double factor);

int main() {
    try {
        auto shapes = createShapes();

        std::cout << std::fixed << std::setprecision(2);

        std::cout << "Shapes before scaling:" << "\n";
        printShapes(shapes);

        scaleAllShapes(shapes, 2.0);

        std::cout << "Shapes after scaling (x2):" << "\n";
        printShapes(shapes);

    }
    catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

std::vector<std::unique_ptr<Shape>> createShapes() {
    std::vector<std::unique_ptr<Shape>> shapes;

    // Прямоугольники
    shapes.push_back(std::make_unique<Rectangle>(
        Point(0.0, 0.0), Point(4.0, 3.0)));

    shapes.push_back(std::make_unique<Rectangle>(
        Point(5.0, 2.0), Point(8.0, 5.0)));

    // Трапеции
    shapes.push_back(std::make_unique<Trapezoid>(
        Point(1.0, 1.0), 6.0, 4.0, 3.0));

    shapes.push_back(std::make_unique<Trapezoid>(
        Point(-2.0, -1.0), 5.0, 3.0, 2.0));

    // Составная фигура
    auto composite = std::make_unique<CompositeShape>();
    composite->addShape(std::make_unique<Rectangle>(
        Point(2.0, 2.0), Point(5.0, 4.0)));
    composite->addShape(std::make_unique<Trapezoid>(
        Point(3.0, 0.0), 4.0, 2.0, 2.0));

    shapes.push_back(std::move(composite));

    return shapes;
}

void printShapes(const std::vector<std::unique_ptr<Shape>>& shapes) {
    for (const auto& shape : shapes) {
        std::cout << *shape << "\n";
    }
}

void scaleAllShapes(std::vector<std::unique_ptr<Shape>>& shapes, double factor) {
    for (auto& shape : shapes) {
        shape->scale(factor);
    }
}
