/* ------------------------------------------------------
CMP2801M: Advanced Programming
Driver program for assignment 
Fall 2019

Written by Ayse Kucukyilmaz

This file is a representative test file. 
During marking, we will use the exact same notation 
as provided in the brief, so make sure
you follow that guideline. Also make sure that you don't 
change the main body provided to you here.
Otherwise, your code may not pass the test cases...

GOOD LUCK! 

------------------------------------------------------ */


#include <iostream>
#include <string>
#include <vector>

#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Movable.h"

using namespace std;

int main()
{
	string userCommand;
	vector <Shape*> shapes;     // this one will hold your shapes
	vector <string> parameters; // this one will hold parameters for the commands


	while (userCommand.compare("exit") != 0) 
	{
		cout << "Enter the command: ";
		getline(cin, userCommand);

		char * cstr = new char[userCommand.length() + 1];
		strcpy_s(cstr, userCommand.length()+1, userCommand.c_str());

		char* nextToken = NULL;
		char* token = strtok_s(cstr, " ", &nextToken); // tokenizes the list

		// run loop while not null
		while (token) 
		{
			parameters.push_back(token);
			token = strtok_s(NULL, " ", &nextToken); // get pointer to next token after space
		}

		string command;

		// Checks if any arguments were given
		if (parameters.size() > 0)
		{
			command = parameters[0];
		}
		else
		{
			command = "";
		}
		 
		if (command.compare("addR") == 0) {

			bool exceptionCaught = true;
			int x, y, w, h;

			// Checks if correct number of arguments given
			if (parameters.size() != 5)
			{
				// number of arguments needed not returned to protect against int overflow (though thats very unlikely to happen)
				cout << "Invalid number of arguments for a rectangle. Only 4 arguments needed." << endl 
					<< "Please enter arguments in the form <command> <point x value> <point y value> <width> <height>" << endl; // << parameters.size() - 1 << " given." << endl;
			}
			else
			{
				// Checks to see if parameters can be converted to ints and if ints are within range
				try
				{
					x = stoi(parameters[1].c_str()); // gets x postion
					y = stoi(parameters[2].c_str()); // gets y postion
					h = stoi(parameters[3].c_str()); // gets rectangle's height
					w = stoi(parameters[4].c_str()); // gets rectangle's width

					// Converted to positive in this try block to catch edge case overflow when converting -2 147 483 648 to +ve
					if (h < 0) // Makes sure height and width are positive values to prevent any issues
					{
						cout << "Negative value height will be converted into a positive value." << endl;
						h += -(h * 2);
					}

					if (w < 0) // Makes sure height and width are positive values to prevent any issues
					{
						cout << "Negative values for widthwill be converted into a positive value." << endl;
						w += -(w * 2);
					}


					// Checks if either width and height are 0, if so, throws "error" 0
					if (h == 0 || w == 0) { throw 0; }

					exceptionCaught = false; // returns false if no errors have been caught
				}
				catch (std::invalid_argument) // Returns exception if any of the values can't be converted to integers
				{
					cout << "Invaild input, please try again." << endl;
				}
				catch (std::out_of_range) // Returns exception if any of the values are out of range
				{
					cout << "One or more of the inputs are out of range. Value must be between 0 and 2 147 483 646." << endl;
				}
				catch (int x)
				{
					cout << "Width and height given cannot be 0." << endl;
				}

				// false if no exception has been caught and width and height 
				if (!exceptionCaught)
				{
					// Checks for int overflow, if no overflow, continues
					if ( ((w * h) / h == w) // checks to see if height and width haven't overflown
						&& ((w * 2) + (h * 2)) > 0 // checks for perimeter overflow
						&& ((w + x) >= w && (w + y) >= w && (h + x) >= h && (h + y) >= h)) // Checks to see if points chosen won't make int overflow
					{
						// Creates new rectangle object
						Rectangle* r = new Rectangle(x, y, h, w); 

						// Adds object to shaopes vector
						shapes.push_back(r);

						// Prints out objcets info
						cout << r;
					}
					else { cout << "Integer overflow detected, maybe the rectangle's area or perimeter are too large?" << endl; }
				}
			}
		}
		 
		else if (command.compare("addS") == 0) {
			
			bool exceptionCaught = true;
			int x, y, e;

			// Checks if correct number of arguments given
			if (parameters.size() != 4)
			{
				// number of arguments needed not returned to protect against int overflow (thoguh thats very unlikely to happen
				cout << "Invalid number of arguments for a square. Only 3 arguments needed." << endl
					<< "Please enter arguments in the form <command> <point x value> <point y value> <edge>" << endl; // << parameters.size() - 1 << " given." << endl;
			}
			else
			{
				// Checks to see if parameters can be converted to ints and if ints are within range
				try
				{
					x = stoi(parameters[1].c_str()); // gets x postion
					y = stoi(parameters[2].c_str()); // gets y postion
					e = stoi(parameters[3].c_str()); // gets squares's edge

					// Converted to positive in this try block to catch edge case overflow when converting -2 147 483 648 to +ve
					if (e < 0) // Makes sure edge is a positive value to prevent any issues
					{
						cout << "Negative value for edge will be converted into a positive value." << endl;
						e += -(e * 2);
					}

					// Checks if edge is 0, if so, throws "error" 0
					if (e == 0) { throw 0; } 

					exceptionCaught = false; // returns false if no errors have been caught
				}
				catch (std::invalid_argument) // Returns exception if any of the values can't be converted to integers
				{
					cout << "Invaild input, please try again." << endl;
				}
				catch (std::out_of_range) // Returns exception if any of the values are out of range
				{
					cout << "One or more of the inputs are out of range. Value must be between 0 and 2 147 483 647." << endl;
				}
				catch (int x) // Catches error if integer thrown, returns exception if edge value is 0
				{
					cout << "Edge given cannot be 0." << endl;
				}

				// false if no exception has been caught and width and height 
				if (!exceptionCaught)
				{
					if (((e * e) / e == e) // checks to see if height and width haven't overflown
						&& (e * 4) > e // Checks if perimeter has overflown
						&& ((e + x) >= e && (e + y) >= e)) // Checks to see if points chosen won't make int overflow
					{
						Square* s = new Square(x, y, e); // Creates new rectangle object
						shapes.push_back(s); // Adds object to shaopes vector
						cout << s; // Prints out objcets info
					}
					else { cout << "Integer overflow detected,  maybe the square's area or perimeter is too large?" << endl; }
				}
			}
		}

		else if (command.compare("addC") == 0) {

			bool exceptionCaught = true;
			int x, y, r;

			// Checks if correct number of arguments given
			if (parameters.size() != 4)
			{
				// number of arguments needed not returned to protect against int overflow (thoguh thats very unlikely to happen
				cout << "Invalid number of arguments for a circle. Only 3 arguments needed." << endl
					<< "Please enter arguments in the form <command> <point x value> <point y value> <radius>" << endl; // << parameters.size() - 1 << " given." << endl;
			}
			else
			{
				// Checks to see if parameters can be converted to ints and if ints are within range
				try
				{
					x = stoi(parameters[1].c_str()); // gets x postion
					y = stoi(parameters[2].c_str()); // gets y postion
					r = stoi(parameters[3].c_str()); // gets squares's edge

					if (r < 0) // Makes sure edge is a positive value to prevent any issues
					{
						cout << "Negative value for edge will be converted into a positive value." << endl;
						r += -(r * 2);
					}

					// Converted to positive in this try block to catch edge case overflow when converting -2 147 483 648 to +ve
					if (r == 0) // Checks if radius is 0, if so, throws "error" 0
					{
						throw 0;
					}

					exceptionCaught = false;
				}
				catch (std::invalid_argument) // Returns exception if any of the values can't be converted to integers
				{
					cout << "Invaild input, please try again." << endl;
				}
				catch (std::out_of_range) // Returns exception if any of the values are out of range
				{
					cout << "One or more of the inputs are out of range. Value must be between 0 and 2 147 483 647." << endl;
				}
				catch (int x) // Catches error if integer thrown, returns exception if edge value is 0
				{
					cout << "Radius given cannot be 0." << endl;
				}

				// false if no exception has been caught and width and height 
				if (!exceptionCaught)
				{
					// Checks for int overflow, if no overflow, continues
					if (((r * r) / r == r) // checks to see if height and width haven't overflown
						&& ((r + x) >= r && (r + y) >= r)) // Checks to see if points chosen won't make int overflow
					{
						Circle* c = new Circle(x, y, r); // Creates new rectangle object
						shapes.push_back(c); // Adds object to shaopes vector
						cout << c; // Prints out objcets info
					}
					else { cout << "Integer overflow detected, maybe the circle's area or perimeter is too large?" << endl; }
				}
			}
			
		}

		else if (command.compare("scale") == 0) 
		{

			bool exceptionCaught = false;
			int shapeNo;
			float xScale, yScale;

			// Ensures correct number of arguments passed
			if (parameters.size() != 4)
			{
				// number of arguments needed not returned to protect against int overflow (thoguh thats very unlikely to happen
				cout << "Invalid number of arguments for scaling. Only 3 arguments needed." << endl; // << parameters.size() - 1 << " given." << endl;
				exceptionCaught = true;
			}
			else
			{
				// Checks to see if string can be converted to ints and if ints are within range
				try
				{
					shapeNo = stoi(parameters[1].c_str()); // gets shape's index
					xScale = stof(parameters[2].c_str()); // gets x scaling
					yScale = stof(parameters[3].c_str()); // gets y scaling

					if (xScale < 0 ) // Makes sure scale values are a positive value
					{
						cout << "Negative value for xScale will be converted into a positive value." << endl;
						xScale += xScale * 2;
					}
					if (yScale < 0) // Makes sure scale values are a positive value
					{
						cout << "Negative value for yScale will be converted into a positive value." << endl;
						yScale += yScale * 2;
					}

				}
				catch (std::invalid_argument & e) // strings can't be converted
				{
					cout << "Invaild input, please try again." << endl;
					exceptionCaught = true;
				}
				catch (std::out_of_range & e) // strings value too large to fit in type
				{
					cout << "One or more of the inputs are out of range." << endl;
					exceptionCaught = true;
				}

				// false if no exception has been caught
				if (!exceptionCaught)
				{
					if (shapes.size() > 0 && (shapeNo - 1) < shapes.size()) // Checks to see if shapes is populated
					{
						// scale object at index... the scaling needs to be isotropic in case of circle and square 
						// you may want to check if the index exists or not!
						Movable* m = dynamic_cast<Movable*>(shapes[shapeNo - 1]);
						Shape* s = shapes[shapeNo - 1];

						if (m) // checks if object exists
						{
							m->scale(xScale, yScale); // scales shape
							cout << s->toString(); // Prints out new shape info
						}
						else
						{
							cout << "No shape exists at that index" << endl;
						}

					}
					else
					{
						cout << "No objects exist yet to scale." << endl;
					}
				}
			}
		}


		else if (command.compare("move") == 0) {
			// move object at index 

			bool exceptionCaught = false;
			int shapeNo, newX, newY;

			// Ensures correct number of arguments passed
			if (parameters.size() != 4)
			{
				// number of arguments needed not returned to protect against int overflow (thoguh thats very unlikely to happen
				cout << "Invalid number of arguments for scaling. Only 3 arguments needed." << endl; // << parameters.size() - 1 << " given." << endl;
				exceptionCaught = true;
			}
			else
			{
				// Checks to see if string can be converted to ints and if ints are within range
				try
				{
					shapeNo = stoi(parameters[1].c_str()); // gets shape's index
					newX = stof(parameters[2].c_str()); // gets y scaling
					newY = stof(parameters[3].c_str()); // gets x scaling
				}
				catch (std::invalid_argument) // strings can't be converted
				{
					cout << "Invaild input, please try again." << endl;
					exceptionCaught = true;
				}
				catch (std::out_of_range) // strings value too large to fit in type
				{
					cout << "One or more of the inputs are out of range." << endl;
					exceptionCaught = true;
				}

				// false if no exception has been caught
				if (!exceptionCaught)
				{
					if (shapes.size() > 0 && (shapeNo - 1) < shapes.size()) // Checks to see if shapes is populated
					{

						Movable* m = dynamic_cast<Movable*>(shapes[shapeNo - 1]);
						Shape* s = shapes[shapeNo - 1];

						m->move(newX, newY);
						cout << s->toString(); // shapes[shapeNo - 1]->toString();
					}
					else
					{
						cout << "No shape exists at that index" << endl;
					}
				}
			}
		}

		else if (command.compare("display") == 0) {
			
			bool exceptionCaught = false;

			// Checks how many arguments are passed
			// If none are passed then it displays all shapes if there are any shapes in the vector
			// If there are no shapes in the vector then a message will appear stating that no shapes to display
			if (shapes.size() > 0) // checks if there are any shapes in the list
			{
				if (parameters.size() > 1) // checks if the user has passed any indexes
				{
					int ind; // stores shapes index

					cout << endl; // to keeping the console tidy
					for (int i = 1; i < parameters.size(); i++)
					{	
						try
						{
							ind = stoi(parameters[i].c_str());
						}
						catch (std::invalid_argument & e) // strings can't be converted
						{
							cout << "Input: " << parameters[i] << " is not a vaild index." << endl;
							exceptionCaught = true;
						}
						catch (std::out_of_range & e) // strings value too large to fit in type
						{
							cout << "Input: " << parameters[i] << " is out of range." << endl;
							exceptionCaught = true;
						}
						if (!exceptionCaught)
						{
							if (ind <= shapes.size() && ind > 0) //  stoi(parameters[i].c_str()) 
							{
								cout << "Shape at index " << ind << ": " << endl;
								cout << endl << shapes.at(ind - 1)->toString() << endl; //  stoi(parameters[i].c_str())
							}
							else
							{
								cout << "No object stored at " << ind << ". Please note that all the list's index starts at 1." << endl;
								// NOTE: did this as user can input many indcies to print, and the program won't print ones that wont work
							}
						}
					}
				}
				else // print all objects
				{
					cout << endl; // to keeping the console tidy
					for (int i = 0; i < shapes.size(); i++) 
					{
						cout << shapes.at(i)->toString() << endl;
					}
				}
			}
			else { cout << "No shapes to display." << endl; }
		}

		else if (command.compare("exit") == 0) {

			// Clears out parameters, cstr and shapes
			parameters.clear();
			delete[] cstr;

			for (int i = 0; i < shapes.size(); i++)
			{
				delete shapes[i];
			}

			break;
		}

		else {
			cout << "Incorrect input, please try again.";
		}

		// Clears out parameters, cstr
		parameters.clear();
		delete[] cstr;

		cin.clear(); // clears cin ready for use, otherwise will always think input is incorrect
		
			
		cout << endl << endl;
	}

	cout << "Press any key to continue...";
	std::getchar();

	return 0;
}
