#include <iostream>
#include <vector>
#include "Point.h"
#include "Shape.h"

// Shape::Shape() {}

Shape::Shape(int ltx, int lty, bool isC, double a, double p)
	: leftTop(ltx, lty), isCircular(isC), area(a), perimeter(p)
{
	points = new std::vector<Point>();
}

Shape::~Shape()
{
	delete points;
}

void Shape::calculateShape()
{
	area = calculateArea();
	perimeter = calculatePerimeter();
	calculatePoints();
}

// return leftTop point object
// Point Shape::getLeftTop() { return leftTop.getPoint(); }

// sets new value as leftTop point object
void Shape::setLeftTop(Point newLeftTop)
{
	leftTop = newLeftTop;
}

