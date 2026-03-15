#ifndef COMPOSITE_SHAPE_HPP
#define COMPOSITE_SHAPE_HPP
#include "shape.hpp"
#include <vector>
#include <memory>
class CompositeShape :public Shape {
public:
	CompositeShape() = default;
	void addShape(std::shared_ptr<Shape> shape);
	double getArea() const override;
	Point getCenter() const override;
	void move(double dx, double dy) override;
	void scale(double coefficient) override;

	std::string getName() const override;
	FrameRect getFrameRect() const override;

	size_t getSize() const;
	std::shared_ptr<Shape> getShapeAtIndex(size_t index) const;
private:
	std::vector<std::shared_ptr<Shape>> shapes_;

};

#endif