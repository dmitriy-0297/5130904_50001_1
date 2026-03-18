
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "rectangle.hpp"
#include "ellipse.hpp"
#include "compositeShape.hpp"

void printShapeInfo(const Shape* shape);

int main()
{
    std::string line;
    std::getline(std::cin, line);

    if (!line.empty()) {
        std::cerr << "Invalid Input";
         return EXIT_FAILURE;
    }

    Rectangle* r1 = new Rectangle(Point(0, 0), Point(4, 3));
    Rectangle* r2 = new Rectangle(Point(1, 2), Point(4, 5));
    Rectangle* r3 = new Rectangle(Point(-3, 4), Point(-2, 5));
    Ellipse* e1 = new Ellipse(Point(2, 4), 2, 1);
    Ellipse* e2 = new Ellipse(Point(6, 2), 1, 2);
    Ellipse* e3 = new Ellipse(Point(-2, -2), 5, 5);
    CompositeShape* c1 = new CompositeShape();
    c1->addShape(r1);
    c1->addShape(e1);

    std::vector<Shape*> shapes;
    shapes.push_back(r2);
    shapes.push_back(r3);
    shapes.push_back(e2);
    shapes.push_back(e3);
    shapes.push_back(c1);

    for (Shape* shape : shapes) {
        printShapeInfo(shape);
    }

    for (Shape* shape : shapes) {
        shape->scale(2.0);
    }

    for (Shape* shape : shapes) {
        printShapeInfo(shape);
    }

    for (Shape* shape : shapes) {
        delete shape;
    }

    return EXIT_SUCCESS;
}


void printShapeInfo(const Shape* shape) {

    const CompositeShape* composite = dynamic_cast<const CompositeShape*>(shape);

    if (composite) {

        composite->printDetails();
    }

    else {

        Point center = shape->getCenter();
        std::cout << "[" << shape->getName() << ", ("
            << std::fixed << std::setprecision(2) << center.x << ", "
            << center.y << "), "
            << shape->getArea() << "]" << std::endl;
    }
}




