#ifndef ADDITIONAL_SHAPES_H
#define ADDITIONAL_SHAPES_H

#include "shape.h"
#include "rectangle.h"
#include <vector>
#include <memory>
#include <string>
#include <iostream>

//êëàññ êðóã
class Circle : public Shape {
private:
    Point center_;    // Öåíòð êðóãà
    double radius_;   // Ðàäèóñ êðóãà

public:
    //äëÿ öåíòðà è ðàäèóñà
    Circle(Point c, double r);

    //ñ÷èòàåì ïëîùàäü
    double getArea() const override;

    //ñ÷èòàåì öåíòð
    Point getCenter() const override;

    void move(double dx, double dy) override;

    void scale(double coefficient) override;

    std::string getName() const override;

    void getBounds(Point& minPt, Point& maxPt) const override;

    void print(std::ostream& os) const override;
};

//êëàññ ñîñòàâíîé ôèãóðû
class CompositeShape : public Shape {
private:
    //óìíûå óêàçàòåëè
    std::vector<std::unique_ptr<Shape>> shapes_;

public:
    CompositeShape() = default;

    //äîáàâëÿåì ôèãóðó â ñîñòàâíóþ
    void addShape(std::unique_ptr<Shape> shape);

    //ñ÷èòàåì ïëîùàäü
    double getArea() const override;

    //ñ÷èòàåì öåíòð
    Point getCenter() const override;

    void move(double dx, double dy) override;

    void scale(double coefficient) override;

    std::string getName() const override;

    //ïîëó÷àåì ïðÿìîóãîëüíèê îãðàíè÷.
    void getBounds(Point& minPt, Point& maxPt) const override;

    void print(std::ostream& os) const override;
};

#endif
