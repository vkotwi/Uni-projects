#include "Movable.h"
#include <iostream>

Point::Point()
	: x(0), y(0)
{ }

// Constructor with points
Point::Point(int pointX, int pointY)
	: x(pointX), y(pointY)
{ }

// Deconstuctor
Point::~Point() {}

// returns point object
Point Point::getPoint() { return { x, y }; }

int Point::readPointX() { return x; } // return Point object's x value
int Point::readPointY() { return y; } // return Point object's y value