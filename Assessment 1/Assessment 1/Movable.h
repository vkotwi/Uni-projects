#pragma once
/*
Rectangle, Square and Circle will extend the abstract Movable class. Movable will have two functions:

move(int newX, int newY) will shift a shape to a new coordinate by updating the
leftTop and recalculating the other points.

scale(float scaleX, float scaleY) will scale a shape in two dimensions. Two 
parameters will determine the scaling factors in each direction. For example a 
call to scale(2,1) will stretch the object in the x direction by a factor of 
2, whereas the y direction will not be scaled. The scale operation will update 
the leftTop and recalculate the other points.

Please assume that Spheres and Squares, scale is strictly isotropic: The scaleX 
and scaleY parameters can only be the same. Your code can output an error or 
handle it appropriately in such a case. Please note that scale affects the area 
and circumference of the objects, which should be handled by your code.
*/

/*
TODO:
- check edge cases (too big for int, no negtives)
*/

#ifndef MOVABLE_H
#define MOVABLE_H

#include "Shape.h"
#include "Point.h"

class Movable
{
public:
	// Changes the shape's Points to the new one
	virtual void move(int, int) = 0; // newX, newY

	// Changes the size of the object
	virtual void scale(float, float) = 0; // scaleX, scaleY
};

#endif // !MOVABLE_H