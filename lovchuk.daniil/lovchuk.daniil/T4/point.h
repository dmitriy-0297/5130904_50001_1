#ifndef POINT_H
#define POINT_H

//Point ïîêàçûâàåò òî÷êó íà ïëîñêîñòè (x;y)
struct Point {
    double x;
    double y;

    Point() : x(0), y(0) {}

    //èíèöèàëèçèðóåì òî÷êó çàäàííûìè êîîðäèíàòàìè
    Point(double xVal, double yVal) : x(xVal), y(yVal) {}

    //ñëîæåíèå
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    //âû÷èòàíèå
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    //óìíîæåíèå íà ñêàëÿð
    Point operator*(double scalar) const {
        return Point(x * scalar, y * scalar);
    }
};

#endif
