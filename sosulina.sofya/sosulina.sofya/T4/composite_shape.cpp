#include "composite_shape.h"

CompositeShape::CompositeShape()
{
}

CompositeShape::~CompositeShape()
{
}

void CompositeShape::addShape(std::shared_ptr<Shape> shape)
{
    shapes_.push_back(shape);
}

double CompositeShape::getArea() const
{
    double totalArea = 0.0;
    for (const auto& shape : shapes_) {
        totalArea += shape->getArea();
    }
    return totalArea;
}

Point CompositeShape::getCenter() const
{
    std::pair<Point, Point> bounds = getBoundingBox();
    double centerX = (bounds.first.x_ + bounds.second.x_) / 2.0;
    double centerY = (bounds.first.y_ + bounds.second.y_) / 2.0;
    return Point(centerX, centerY);
}

void CompositeShape::move(const Point& delta)
{
    for (auto& shape : shapes_) {
        shape->move(delta);
    }
}

void CompositeShape::scale(double factor)
{
    Point center = getCenter();
    for (auto& shape : shapes_) {
        Point shapeCenter = shape->getCenter();
        double newX = center.x_ + (shapeCenter.x_ - center.x_) * factor;
        double newY = center.y_ + (shapeCenter.y_ - center.y_) * factor;
        Point delta(newX - shapeCenter.x_, newY - shapeCenter.y_);
        shape->move(delta);
        shape->scale(factor);
    }
}

std::string CompositeShape::getName() const
{
    return "COMPOSITE";
}

std::pair<Point, Point> CompositeShape::getBoundingBox() const
{
    if (shapes_.empty()) {
        return std::make_pair(Point(0, 0), Point(0, 0));
    }
    double minX = 1e9, minY = 1e9;
    double maxX = -1e9, maxY = -1e9;
    for (const auto& shape : shapes_) {
        auto bounds = shape->getBoundingBox();
        if (bounds.first.x_ < minX) minX = bounds.first.x_;
        if (bounds.first.y_ < minY) minY = bounds.first.y_;
        if (bounds.second.x_ > maxX) maxX = bounds.second.x_;
        if (bounds.second.y_ > maxY) maxY = bounds.second.y_;
    }
    return std::make_pair(Point(minX, minY), Point(maxX, maxY));
}

void CompositeShape::print(std::ostream& os) const
{
    Point center = getCenter();
    double area = getArea();
    os << "[" << getName() << ", (";
    os << center.x_ << ", " << center.y_ << "), ";
    os << area << ":\n";
    for (size_t i = 0; i < shapes_.size(); ++i) {
        os << "  ";
        os << shapes_[i]->getName() << ", (";
        Point c = shapes_[i]->getCenter();
        os << c.x_ << ", " << c.y_ << "), ";
        os << shapes_[i]->getArea();
        if (i != shapes_.size() - 1) {
            os << ",";
        }
        os << "\n";
    }
    os << "]";
}
