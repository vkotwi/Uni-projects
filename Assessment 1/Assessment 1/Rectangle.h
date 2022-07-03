#pragma once

// need def if have pragma once?


#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <ostream>
#include <string>
#include "Shape.h"
#include "Movable.h"

class Rectangle : public Shape, public Movable //, public Point
{
	int width, height;

public:
	// Constructor
	Rectangle(int, int, int, int); // x, y, height, width

	// Deconstructor
	~Rectangle();

	// Printing << overload
	friend std::ostream &operator<<(std::ostream &, Rectangle *);

	// virtual override
	float calculateArea();
	float calculatePerimeter();
	void calculatePoints(); // std::vector<Point>

	// Functions
	virtual void move(int, int); // newX, newY
	virtual void scale(float, float); // scaleX, scaleY
	virtual std::string toString();
	

};

#endif // !RECTANGLE_H