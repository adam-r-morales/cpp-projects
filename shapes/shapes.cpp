#include <iostream>
#include "Header.h"
#include <cmath>

Shape::Shape() : x_(0.0), y_(0.0), z_(0.0)
{

}

Shape::Shape(float x, float y, float z) : x_(x), y_(y), z_(z)
{
	
}

Shape::~Shape()
{
	
}

const char* Shape::Name() const
{
	return "Shape!";
}

float Shape::Volume() const
{
	return 0.0;
}

float Shape::Area() const
{
	return 0.0;
}

Box::Box() : Shape::Shape()
{

}

Box::Box(float width, float length, float height) : Shape::Shape(width, length, height)
{
	
}

Box::~Box()
{
	
}

const char* Box::Name() const
{
	return "Box!";
}

float Box::Volume() const
{
	return x_ * y_ * z_;
}

float Box::Area() const
{
	return 2 * (x_ * y_) + 2 * (x_ * z_) + 2 * (y_ * z_);
}

Cylinder::Cylinder() : Shape::Shape()
{

}

Cylinder::Cylinder(float radius, float height) : Shape::Shape(radius, height, 0.0)
{
	
}

Cylinder::~Cylinder()
{
	
}

const char* Cylinder::Name() const
{
	return "Cylinder!";
}

float Cylinder::Volume() const
{
	return pi * x_ * x_ * y_;
}

float Cylinder::Area() const
{
	return (2 * pi * x_ * y_) + (2 * pi * x_ * x_);
}

Rectangle::Rectangle() : Shape::Shape()
{

}

Rectangle::Rectangle(float width, float length) : Shape::Shape(width, length, 0.0)
{
	
}

Rectangle::~Rectangle()
{
	
}

const char* Rectangle::Name() const
{
	return "Rectangle!";
}

float Rectangle::Area() const
{
	return x_ * y_;
}