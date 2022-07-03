#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Rectangle.h"
#include "Movable.h"

// Constructor
Rectangle::Rectangle(int x, int y, int h, int w) 
	: Shape(x, y, false, area, perimeter), height(h), width(w)
{
	// sets Rectangles's area, perimeter and points
	area = calculateArea();
	perimeter = calculatePerimeter();
	calculatePoints();
}


// Deconstructor
Rectangle::~Rectangle()
{
	// Destroy points
	delete points;
}

// returns area of rectangle
float Rectangle::calculateArea()
{ 
	return width * height;
}

// returns perimeter of rectangle
float Rectangle::calculatePerimeter()
{
	return ((width * 2) + (height * 2));
}

// Creates and assigns shape's points to Point a
void Rectangle::calculatePoints() // std::vector<Point>
{
	points->clear(); // cleans out vector for new Points
	// Creates point objects with coordinates and pushes them into an array
	Point tR = Point( leftTop.readPointX() + width, leftTop.readPointY());
	Point bL = Point( leftTop.readPointX(), leftTop.readPointY() + height);
	Point bR = Point( leftTop.readPointX() + width, leftTop.readPointY() + height);

	// Stores point objects in vector
	(*points).push_back(leftTop.getPoint()); (*points).push_back(tR); (*points).push_back(bR); (*points).push_back(bL);

	// return points;
}

void Rectangle::move(int newX, int newY)
{
	setLeftTop(Point(newX, newY)); // Creates and sets new top left Point

	// or should I use set point?
	calculatePoints(); // Sets new Points // points = 
}

void Rectangle::scale(float scaleX, float scaleY)
{
	// new width and heigth of object
	width *= scaleX ;
	height *= scaleY;

	// recalculates shape's area, perimeter and points
	calculateShape();
}

std::ostream& operator<<(std::ostream& out, Rectangle* r)
{
	out << std::fixed;
	out << std::setprecision(1);
	out << "Rectangle [h=" << (int)std::round(r->height) << ",w=" << (int)std::round(r->width) << "]" << std::endl;
	out << "Points [";

	for (int i = 0; i < 4; i++)
	{
		out << "(" << r->points->at(i).readPointX() << "," << r->points->at(i).readPointY() << ")";
	}
	out << "]" << std::endl;

	// std::setprecision(1) <<
	out << "Area=" << r->area<< " Perimeter=" << r->perimeter << std::endl;

	return out;
}

// Gets information on Rectangle object, converts it to a string and returns that sting
std::string Rectangle::toString()
{
	// Return shape type, dimensions, coordinates of points, area and perimeter
	typedef std::string str;

	// adds text to string and converts const char and int to strings
	str s = str("Rectangle [h=") + std::to_string((int)round(height))
		+ str(", w=") + std::to_string((int)round(width)) + str("]\n")
		+ str("Points [");

	// prints out each point stored in Shape's Point vector
	for (int i = 0; i < 4; i++)
	{
		s += str("(") + std::to_string(points->at(i).readPointX())
			+ str(", ") + std::to_string(points->at(i).readPointY())
			+ str(")");
	}

	s += "]\n";

	std::stringstream strAreaPeri;
	strAreaPeri << std::fixed << std::setprecision(1) << "Area = " << area << " Perimeter = " << perimeter;

	s += strAreaPeri.str() + "\n";

	return s;
}
