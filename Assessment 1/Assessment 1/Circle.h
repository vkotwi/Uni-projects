#pragma once

// need def if have pragma once?
#ifndef CIRCLE_H
#define CIRCLE_H

#include "Shape.h"
#include "Movable.h"

class Circle : public Shape, public Movable
{
	int radius;

public:
	// Constructor
	Circle(int, int, int); // x, y, radius

	// Deconstructor
	~Circle();

	// Printing << overload
	friend std::ostream &operator<<(std::ostream &, Circle *);

	// virtual override
	float calculateArea();
	float calculatePerimeter();
	void calculatePoints(); // std::vector<Point>

	// Functions
	virtual void move(int, int); // newX, newY
	virtual void scale(float, float); // scaleX, ScaleY
	virtual std::string toString();
};

#endif // !CIRCLE_H
