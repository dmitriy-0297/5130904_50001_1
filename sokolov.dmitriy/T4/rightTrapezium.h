#ifndef RIGHT_TRAPEZIUM_H
#define RIGHT_TRAPEZIUM_H
#include "shape.h"
#include "point.h"
#include <string>

class RightTrapezium : public Shape {
public:

    RightTrapezium(const Point&, const double, const double, const double);

    virtual double getArea() const override;

    virtual Point getCenter() const override;

    virtual void move(const double, const double) override;

    virtual void scale(const double) override;

    virtual std::string getName() const override;

    virtual std::vector<Point> getPoints() const override;

    virtual ~RightTrapezium() = default;

private:

    Point leftDown_;

    double longBase_;
    double shortBase_;
    double height_;

};

#endif
