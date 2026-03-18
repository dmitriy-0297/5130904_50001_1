#ifndef SHAPE_H
#define SHAPE_H

#include "point.h"
#include <string>
#include <iostream>

class Shape {
public:
    virtual ~Shape() = default;

    //ïëîùàäü
    virtual double getArea() const = 0;

    //êîîðäèíàòû öåíòðà
    virtual Point getCenter() const = 0;

    //ïåðåìåùåíèå ôèãóðû
    virtual void move(double dx, double dy) = 0;

    virtual void scale(double coefficient) = 0;

    //íàçâàíèÿ ôèãóðû ("RECTANGLE", "CIRCLE")
    virtual std::string getName() const = 0;

    //îãðàíè÷èâàþùèé ïðÿìîóãîëüíèê (ìèí è ìàêñ)
    virtual void getBounds(Point& minPt, Point& maxPt) const = 0;

    virtual void print(std::ostream& os) const = 0;
};

#endif
