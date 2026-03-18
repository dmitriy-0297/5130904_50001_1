#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "shape.h"

//êëàññ ïðÿìîóãîëüíèê
class Rectangle : public Shape {
private:
    Point bottomLeft_;//ëåâûé íèæíèé óãîë
    Point topRight_;//ïðàâûé âåðõíèé óãîë

public:
    //äëÿ ïðèåìà óãëîâ
    Rectangle(Point bl, Point tr);

    //ïëîùàäü
    double getArea() const override;

    //öåíòð
    Point getCenter() const override;

    //ïåðåìåùåíèå
    void move(double dx, double dy) override;

    //ìàñøòàáèðîâàíèå îòíîñèòåëüíî öåíòðà
    void scale(double coefficient) override;

    //âîçâðàùàåò íàçâàíèå
    std::string getName() const override;

    //îãðàí÷èâàþùèé ïðÿìîóãîëüíèê (ìèí è ìàêñ)
    void getBounds(Point& minPt, Point& maxPt) const override;

    void print(std::ostream& os) const override;
};

#endif
