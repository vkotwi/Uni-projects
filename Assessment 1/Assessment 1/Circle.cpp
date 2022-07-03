#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Circle.h"
#include "Movable.h"

#define PI 3.14159 // sets value for pi

Circle::Circle(int x, int y, int r) 
	: Shape(x, y, true, area, perimeter), radius(r)
{
	// sets Circle's area, perimeter and points
	area = calculateArea();
	perimeter = calculatePerimeter();
	calculatePoints();  
}

Circle::~Circle() 
{
	delete points;
}

float Circle::calculateArea()
{
	return (PI * (radius * radius));
}

float Circle::calculatePerimeter()
{
	return (2 * radius * PI);
}


// Creates and stores circle's points to Point vector
void Circle::calculatePoints() // std::vector<Point>
{
	// Creates point objects with coordinates and pushes them into an array
	Point bR = Point(leftTop.readPointX() + (2 * radius), leftTop.readPointY() + (2 * radius));

	// Stores point objects in vector
	(*points).push_back(leftTop.getPoint()); (*points).push_back(bR);
	// (*points).push_back(tR);  (*points).push_back(bL);

	// return points;
}

void Circle::move(int newX, int newY)
{
	setLeftTop(Point(newX, newY)); // Creates and sets new top left Point
	points->clear(); // cleans out vector for new Points
	calculatePoints(); // Sets new Points // points = 
}

void Circle::scale(float scaleX, float scaleY)
{
	if (scaleX != scaleY)
	{
		std::cout << "Incorrect input, isotropic shapes require both inputs to be equal." << std::endl;
		return;
	}
	// new edge of object set
	radius*= scaleX;

	// recalculates shape's area, perimeter and points
	calculateShape();
}

std::ostream& operator<<(std::ostream& out, Circle* c)
{
	out << std::fixed;
	out << std::setprecision(1);
	out << "Circle [r=" << c->radius << "]" << std::endl;
	out << "Points [";

	// gets x and y coordinate of points to print out
	for (int i = 0; i < 2; i++)
	{
		out << "(" << c->points->at(i).readPointX() << "," << c->points->at(i).readPointY() << ")";
	}
	out << "]" << std::endl;

	// std::setprecision(1) <<
	out << "Area=" << c->area << " Perimeter=" << c->perimeter << std::endl;

	return out;
}

// Gets information on Circle object, converts it to a string and returns that sting
std::string Circle::toString()
{
	// Returns shape type, dimensions, coordinates of corner points, area and perimeter
	typedef std::string str;

	// adds text to string and converts const char and int to strings
	str s = str("Circle [r=") + std::to_string((int)round(radius)) + str("]\n")
		+ str("Points [");

	// prints out each point stored in Shape's Point vector
	for (int i = 0; i < 2; i++)
	{
		s += str("(") + std::to_string(points->at(i).readPointX())
			+ str(", ") + std::to_string(points->at(i).readPointY())
			+ str(")");
	}

	s += "]\n";

	std::stringstream strAreaPeri;
	strAreaPeri<< std::fixed << std::setprecision(1) << "Area = " << area << " Perimeter = " << perimeter;

	s += strAreaPeri.str() + "\n";

	return s;
}
