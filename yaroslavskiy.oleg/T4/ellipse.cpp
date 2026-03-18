
#include "ellipse.hpp"


Ellipse::Ellipse(const Point& center, double rx, double ry)
    : centerO (center), radiusX(rx), radiusY(ry) {
}

double Ellipse::getArea() const {
    return 3.1415926 * radiusX * radiusY;
}

Point Ellipse::getCenter() const {
    return centerO;
}

void Ellipse::move(double dx, double dy) {
    centerO.x += dx;
    centerO.y += dy;
}

void Ellipse::scale(double coef) {
    radiusX *= coef;
    radiusY *= coef;
}

std::string Ellipse::getName() const {
    return "ELLIPSE";
}



