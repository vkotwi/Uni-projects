#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "Square.h"
#include "Movable.h"


Square::Square(int x, int y, int e) 
	: Shape(x, y, false, area, perimeter), edge(e)
{
	// sets Square's area, perimeter and points
	area = calculateArea();
	perimeter = calculatePerimeter();
	calculatePoints();
}

Square::~Square() 
{
	delete points;
}

float Square::calculateArea()
{
	return edge * edge;
}

float Square::calculatePerimeter()
{
	return edge * 4;
}

void Square::calculatePoints() // std::vector<Point>
{
	Point tR = Point(leftTop.readPointX() + edge, leftTop.readPointY());
	Point bL = Point(leftTop.readPointX(), leftTop.readPointY() + edge);
	Point bR = Point(leftTop.readPointX() + edge, leftTop.readPointY() + edge);

	// Stores point objects in vector
	(*points).push_back(leftTop.getPoint()); (*points).push_back(tR); (*points).push_back(bR); (*points).push_back(bL);

	// return points;
}

void Square::move(int newX, int newY)
{
	setLeftTop(Point(newX, newY)); // Creates and sets new top left Point
	points->clear(); // cleans out vector for new Points
	calculatePoints(); // Sets new Points 
}

void Square::scale(float scaleX, float scaleY)
{
	// checks if scale sare the same as the object is isometric
	if (scaleX != scaleY)
	{
		std::cout << "Isotropic shapes require both inputs to be equal, please try again." << std::endl;
		return;
	}

	// new edge of object set
	edge *= scaleX;

	// recalculates shape's area, perimeter and points
	calculateShape();
}


std::ostream& operator<<(std::ostream& out, Square* e)
{

	out << std::fixed;
	out << std::setprecision(1);
	out << "Square [e=" << (int)std::round(e->edge) << "]" << std::endl;
	out << "Points [";

	for (int i = 0; i < 4; i++)
	{
		out << "(" << e->points->at(i).readPointX() << "," << e->points->at(i).readPointY() << ")";
	}
	out << "]" << std::endl;

	// std::setprecision(1) <<
	out << "Area=" << e->area << " Perimeter=" << e->perimeter << std::endl;

	return out;
}

// Gets information on Square object, converts it to a string and returns that sting
std::string Square::toString()
{
	// Needs to return shape type, dimensions, coordinates of corner points, area and perimeter
	// check end of brief for how to format
	typedef std::string str;

	// adds text to string and converts const char and int to strings
	str s = str("Square [e=") + std::to_string((int)round(edge)) + str("]\n")
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
