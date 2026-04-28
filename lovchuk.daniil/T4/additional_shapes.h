#ifndef ADDITIONAL_SHAPES_H
#define ADDITIONAL_SHAPES_H

#include "shape.h"
#include "rectangle.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>

//класс круг
class Circle : public Shape {
private:
    Point center_;    // ÷ентр круга
    double radius_;   // –адиус круга

public:
    //дл€ центра и радиуса
    Circle(Point c, double r);

    //считаем площадь
    double getArea() const override;

    //считаем центр
    Point getCenter() const override;

    void move(double dx, double dy) override;

    void scale(double coefficient) override;

    std::string getName() const override;

    void getBounds(Point& minPt, Point& maxPt) const override;

    void print(std::ostream& os) const override;
};

//класс составной фигуры
class CompositeShape : public Shape {
private:
    //умные указатели
    std::vector<std::unique_ptr<Shape>> shapes_;

public:
    CompositeShape() = default;

    //добавл€ем фигуру в составную
    void addShape(std::unique_ptr<Shape> shape);

    //считаем площадь
    double getArea() const override;

    //считаем центр
    Point getCenter() const override;

    void move(double dx, double dy) override;

    void scale(double coefficient) override;

    std::string getName() const override;

    //получаем пр€моугольник огранич.
    void getBounds(Point& minPt, Point& maxPt) const override;

    void print(std::ostream& os) const override;
};

#endif 