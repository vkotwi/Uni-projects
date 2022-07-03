#pragma once

#ifndef POINT_H
#define POINT_H

#include <vector>

// coordinates class, where object is on grid.
class Point 
{
	// set to private by default
	int x, y;

public: 
	// Constructer
	Point();
	Point(int pointX, int pointY);

	// Deconstructer
	~Point();

	Point getPoint();

	// how else to return a printable value???
	int readPointX(); // { return x; }
	int readPointY(); // { return y; }
};

#endif // !POINT_H