#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "shape.hpp"

class Rectangle : public Shape {

public:

	Rectangle(const Point& leftBottom, const Point& rightTop);

	double getArea() const override;
	Point getCenter() const override;
	void move(double dx, double dy) override;
	void scale(double coefficient) override;

	std::string getName() const override;
	FrameRect getFrameRect() const override;

private:

	Point leftBottom_;
	Point rightTop_;

};

#endif
