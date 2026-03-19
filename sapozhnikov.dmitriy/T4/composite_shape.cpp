#include "composite_shape.hpp"
#include <limits>

void CompositeShape::addShape(std::shared_ptr<Shape> shape) {
	if (shape) {
		shapes_.push_back(shape);
	}

}
double CompositeShape::getArea() const {
	double totalArea = 0;
	for (const auto& shape:shapes_ ) {
		totalArea += shape->getArea();
	}
	return totalArea;

};
FrameRect CompositeShape::getFrameRect() const {

	if (shapes_.empty()) {
		return { 0, 0, 0, 0 };
	}

	double minX = std::numeric_limits<double>::max();
	double maxX = std::numeric_limits<double>::lowest();
	double minY = std::numeric_limits<double>::max();
	double maxY = std::numeric_limits<double>::lowest();

	for (const auto& shape : shapes_) {

		FrameRect frame = shape->getFrameRect();
		double halfW = frame.width / 2.0;
		double halfH = frame.height / 2.0;

		minX = std::min(minX, frame.x - halfW);
		maxX = std::max(maxX, frame.x + halfW);
		minY = std::min(minY, frame.y - halfH);
		maxY = std::max(maxY, frame.y + halfH);
	}

	return { (minX + maxX) / 2.0, (minY + maxY) / 2.0, maxX - minX, maxY - minY };
};
Point CompositeShape::getCenter() const {
	FrameRect frame = getFrameRect();
	return { frame.x, frame.y };
};
void CompositeShape::move(double dx, double dy) {
	for (auto& shape:shapes_) {
		shape->move(dx, dy);

	}

};
void CompositeShape::scale(double coefficient) {
	Point center = getCenter();

	for (auto& shape : shapes_) {

		Point shapeCenter = shape->getCenter();
		double dx = shapeCenter.x - center.x;
		double dy = shapeCenter.y - center.y;


		shape->move(dx * (coefficient - 1), dy * (coefficient - 1));

		shape->scale(coefficient);
	}

};

std::string CompositeShape::getName() const {
	return "COMPOSITE";

};

size_t CompositeShape::getSize() const {
	return shapes_.size();
};
std::shared_ptr<Shape> CompositeShape::getShapeAtIndex(size_t index) const {
	return shapes_.at(index);
};