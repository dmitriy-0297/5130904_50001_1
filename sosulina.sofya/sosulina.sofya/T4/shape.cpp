#include "shape.h"

int Shape::counter_ = 0;

Shape::Shape() :
    id_(0)
{
    counter_++;
    id_ = counter_;
}

Shape::~Shape()
{
}

int Shape::getId() const
{
    return id_;
}
