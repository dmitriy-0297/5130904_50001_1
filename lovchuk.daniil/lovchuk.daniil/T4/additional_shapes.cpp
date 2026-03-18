#include "additional_shapes.h"
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <limits>

using namespace std;

//ðåàëèçàöèÿ Circle
Circle::Circle(Point c, double r)
    : center_(c), radius_(r) {
    //ïðîâåðÿåì ðàäèóñ
    if (r <= 0) {
        throw invalid_argument("Ðàäèóñ äîëæåí áûòü ïîëîæèòåëüíûì.");
    }
}

double Circle::getArea() const {
    return 3.14 * radius_ * radius_;
}

Point Circle::getCenter() const {
    return center_;
}

void Circle::move(double dx, double dy) {
    //äâèãàåì öåíòð
    center_.x += dx;
    center_.y += dy;
}

void Circle::scale(double coefficient) {
    //ïðîâåðÿåì êîýôèöèåíò
    if (coefficient <= 0) {
        throw invalid_argument("Êîýôôèöèåíò ìàñøòàáèðîâàíèÿ äîëæåí áûòü ïîëîæèòåëüíûì.");
    }
    radius_ *= coefficient;
}

string Circle::getName() const {
    return "CIRCLE";
}

void Circle::getBounds(Point& minPt, Point& maxPt) const {
    minPt = Point(center_.x - radius_, center_.y - radius_);
    maxPt = Point(center_.x + radius_, center_.y + radius_);
}

void Circle::print(ostream& os) const {
    os << fixed << setprecision(2);
    os << "[" << getName() << ", ("
        << center_.x << ", " << center_.y << "), "
        << getArea() << "]";
}

//ðåàëèçàöèÿ CompositeShape
void CompositeShape::addShape(unique_ptr<Shape> shape) {
    if (!shape) {
        throw invalid_argument("Íåâîçìîæíî äîáàâèòü ïóñòóþ ôèãóðó.");
    }
    shapes_.push_back(std::move(shape));
}

double CompositeShape::getArea() const {
    double area = 0.0;
    for (const auto& shape : shapes_) {
        area += shape->getArea();
    }
    return area;
}

Point CompositeShape::getCenter() const {
    //åñëè ôèãóð íåò, òî âîçâðàùàåì íà÷àëî êîîðäèíàò
    if (shapes_.empty()) {
        return Point(0, 0);
    }
    double minX = numeric_limits<double>::max();
    double minY = numeric_limits<double>::max();
    double maxX = numeric_limits<double>::lowest();
    double maxY = numeric_limits<double>::lowest();

    for (const auto& shape : shapes_) {
        Point minPt, maxPt;
        shape->getBounds(minPt, maxPt);
        if (minPt.x < minX) minX = minPt.x;
        if (minPt.y < minY) minY = minPt.y;
        if (maxPt.x > maxX) maxX = maxPt.x;
        if (maxPt.y > maxY) maxY = maxPt.y;
    }
    return Point((minX + maxX) / 2.0, (minY + maxY) / 2.0);
}

void CompositeShape::move(double dx, double dy) {
    for (auto& shape : shapes_) {
        shape->move(dx, dy);
    }
}

void CompositeShape::scale(double coefficient) {
    if (coefficient <= 0) {
        throw invalid_argument("Êîýôôèöèåíò ìàñøòàáèðîâàíèÿ äîëæåí áûòü ïîëîæèòåëüíûì.");
    }

    if (shapes_.empty()) {
        return;
    }

    Point center = getCenter();
    for (auto& shape : shapes_) {
        Point oldCenter = shape->getCenter();
        double dx = oldCenter.x - center.x;
        double dy = oldCenter.y - center.y;
        double newX = center.x + dx * coefficient;
        double newY = center.y + dy * coefficient;
        shape->move(newX - oldCenter.x, newY - oldCenter.y);
        shape->scale(coefficient);
    }
}

string CompositeShape::getName() const {
    return "COMPOSITE";
}

void CompositeShape::getBounds(Point& minPt, Point& maxPt) const {
    //ôèãóðû íåò - âîçâðàùàåì íóëè
    if (shapes_.empty()) {
        minPt = Point(0, 0);
        maxPt = Point(0, 0);
        return;
    }
    //îáùèé îãðàíè÷èâàþùèé ïðÿìîóãîëüíèê
    double minX = numeric_limits<double>::max();
    double minY = numeric_limits<double>::max();
    double maxX = numeric_limits<double>::lowest();
    double maxY = numeric_limits<double>::lowest();
    for (const auto& shape : shapes_) {
        Point sMin, sMax;
        shape->getBounds(sMin, sMax);
        if (sMin.x < minX) minX = sMin.x;
        if (sMin.y < minY) minY = sMin.y;
        if (sMax.x > maxX) maxX = sMax.x;
        if (sMax.y > maxY) maxY = sMax.y;
    }
    minPt = Point(minX, minY);
    maxPt = Point(maxX, maxY);
}

void CompositeShape::print(ostream& os) const {
    os << fixed << setprecision(2);
    os << "[" << getName() << ", ("
        << getCenter().x << ", " << getCenter().y << "), "
        << getArea() << ":\n";
    for (const auto& shape : shapes_) {
        os << " " << shape->getName() << ", ("
            << shape->getCenter().x << ", " << shape->getCenter().y << "), "
            << shape->getArea() << ",\n";
    }
    os << "]";
}
