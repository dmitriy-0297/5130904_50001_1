#include "rectangle.h"
#include <stdexcept>
#include <iomanip>

using namespace std;

Rectangle::Rectangle(Point bl, Point tr)
    : bottomLeft_(bl), topRight_(tr) {
    //ïðàâåðêà íà ïðàâèëüíîñòü êîîðäèíàò
    if (tr.x <= bl.x || tr.y <= bl.y) {
        throw invalid_argument("Íåêîððåêòíûå êîîðäèíàòû ïðÿìîóãîëüíèêà.");
    }
}

double Rectangle::getArea() const {
    //ñ÷èòàåì ïëîùàäü
    return (topRight_.x - bottomLeft_.x) * (topRight_.y - bottomLeft_.y);
}

Point Rectangle::getCenter() const {
    //ñ÷èòàåì öåíòð
    return Point(
        (bottomLeft_.x + topRight_.x) / 2.0,
        (bottomLeft_.y + topRight_.y) / 2.0
    );
}

void Rectangle::move(double dx, double dy) {
    //ñìåùàåì óãëû
    bottomLeft_.x += dx;
    bottomLeft_.y += dy;
    topRight_.x += dx;
    topRight_.y += dy;
}

void Rectangle::scale(double coefficient) {
    //ïðîâåðÿåì êîýôèöèåíò
    if (coefficient <= 0) {
        throw invalid_argument("Êîýôôèöèåíò ìàñøòàáèðîâàíèÿ äîëæåí áûòü ïîëîæèòåëüíûì.");
    }
    Point center = getCenter();
    double width = topRight_.x - bottomLeft_.x;
    double height = topRight_.y - bottomLeft_.y;
    width *= coefficient;
    height *= coefficient;
    bottomLeft_.x = center.x - width / 2.0;
    bottomLeft_.y = center.y - height / 2.0;
    topRight_.x = center.x + width / 2.0;
    topRight_.y = center.y + height / 2.0;
}

string Rectangle::getName() const {
    return "RECTANGLE";
}

void Rectangle::getBounds(Point& minPt, Point& maxPt) const {
    minPt = bottomLeft_;
    maxPt = topRight_;
}

void Rectangle::print(ostream& os) const {
    os << fixed << setprecision(2);
    os << "[" << getName() << ", ("
        << getCenter().x << ", " << getCenter().y << "), "
        << getArea() << "]";
}
