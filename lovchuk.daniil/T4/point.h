#ifndef POINT_H
#define POINT_H

//Point показывает точку на плоскости (x;y)
struct Point {
    double x;
    double y;

    Point() : x(0), y(0) {}

    //инициализируем точку заданными координатами
    Point(double xVal, double yVal) : x(xVal), y(yVal) {}

    //сложение
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    //вычитание 
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    //умножение на скаляр
    Point operator*(double scalar) const {
        return Point(x * scalar, y * scalar);
    }
};

#endif