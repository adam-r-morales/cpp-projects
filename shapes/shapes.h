#include <iostream>

#ifndef _HEADER_H
#define _HEADER_H


//Declare Shape class
class Shape
{
public:
	Shape();
	Shape(float x, float y, float z);
	virtual ~Shape();
	virtual const char* Name() const;
	virtual float Volume() const;
	virtual float Area() const;

private:
	Shape(const Shape& a);
	Shape& operator =(const Shape& s);

protected:
	float x_, y_, z_;
};

//Declare Box class
class Box : public Shape
{
public:
	Box();
	Box(float width, float length, float height);
	virtual ~Box();
	const char* Name() const;
	float Volume() const;
	float Area() const;

private:
	Box(const Box& a);
	Box& operator =(const Box& b);
};

//Declare Cylinder class
class Cylinder : public Shape
{
public:
	Cylinder();
	Cylinder(float radius, float height);
	virtual ~Cylinder();
	const char* Name() const;
	float Volume() const;
	float Area() const;

private:
	Cylinder(const Cylinder& a);
	Cylinder& operator =(const Cylinder& c);
	float pi = 3.14;
};

//Declare Rectangle class
class Rectangle : public Shape
{
public:
	Rectangle();
	Rectangle(float width, float length);
	virtual ~Rectangle();
	const char* Name() const;
	float Area() const;

private:
	Rectangle(const Rectangle& a);
	Rectangle& operator =(const Rectangle& r);

};

#endif // !_HEADER_H

