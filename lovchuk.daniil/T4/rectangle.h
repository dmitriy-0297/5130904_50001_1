#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

//класс прямоугольник
class Rectangle : public Shape {
private:
    Point bottomLeft_;//левый нижний угол 
    Point topRight_;//правый верхний угол 

public:
    //для приема углов
    Rectangle(Point bl, Point tr);

    //площадь
    double getArea() const override;

    //центр
    Point getCenter() const override;

    //перемещение 
    void move(double dx, double dy) override;

    //масштабирование относительно центра
    void scale(double coefficient) override;

    //возвращает название
    std::string getName() const override;

    //огранчивающий прямоугольник (мин и макс)
    void getBounds(Point& minPt, Point& maxPt) const override;

    void print(std::ostream& os) const override;
};

#endif