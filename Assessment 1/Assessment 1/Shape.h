#pragma once

// TODO: move to shape cpp

#ifndef SHAPE_H
#define SHAPE_H

#include <ostream>
#include <vector>
#include "Point.h"

// implement constructors, destructors, accessor and mutator functions as appropriate



// NOTE: abstract classes are class with virtual functions in them
class Shape : public Point
{
// shows a star in brief so is protected
protected:
	Point leftTop;

	bool isCircular;
	float area;
	float perimeter;
	// std::string shape;
	std::vector<Point> *points;

	// REPORT NOTE: using doubles for accuracy
public:
	// Constructor: calculates area and perimeter in it and gets leftTopX, leftTopY, whether shape is circular, area and perimeter
	Shape(int ltx, int lty, bool isC, double a, double p);

	// Deconstructor
	~Shape();

	// pure virtual functions , so no default - must be initialised in child classes
	virtual void calculateShape(); // Runs the calculate area, perimeter and points functions
	virtual float calculateArea() = 0; // calculates and returns shape's area
	virtual float calculatePerimeter() = 0; // calculates and returns shape's perimeter
	virtual void calculatePoints() = 0; // pure virtul, what does this return??? // std::vector<Point> 
	virtual std::string toString() = 0;

	// get and set leftTop
	// Shape::Point getLeftTop(); // Returns then leftTop Point obj
	void setLeftTop(Point newLeftTop); // Set the leftTop Point obj to a new value
};

#endif // !SHAPE_H