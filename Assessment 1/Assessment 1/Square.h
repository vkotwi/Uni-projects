#pragma once

// need def if have pragma once?
#ifndef SQUARE_H
#define SQUARE_H

#include "Shape.h"
#include "Movable.h"

class Square : public Shape, public Movable //, public Point
{
	int edge;

public:
	// Constructor
	Square(int x, int y, int e); 

	// Deconstructor
	~Square();

	// Printing << overload
	friend std::ostream &operator<<(std::ostream &, Square *);

	// virtual override
	float calculateArea();
	float calculatePerimeter();
	void calculatePoints(); // std::vector<Point>
	
	virtual void move(int, int);
	virtual void scale(float, float);
	virtual std::string toString();

};

#endif // !SQUARE_H
