using System;
using System.Linq;
using System.Text.RegularExpressions;

class Program
{
	static void Main()
	{
		// Variables list
		string[] hexValue;
		string[] letters = { "a", "b", "c", "d", "e", "f" };
		bool flag;
		bool cont = true;

		// Loops till user exits program
		while (cont)
		{
			// Resets variables on rerun
			string[] hexInverted = { "", "", "" };
			int[] rgbValue = { 0, 0, 0 };
			int[] rgbInverted = { 0, 0, 0 };
			int counter = 0;

			// Loops till gets a valid response
			while (true)
			{
				flag = true;
				Console.Write("Please enter a colour's hexadecimal value: ");

				hexValue = Regex.Replace(Console.ReadLine(), "[^A-Za-z0-9]", "").ToLower().ToCharArray() // Makes char array to get each character
					.Select(c => c.ToString()).ToArray();// Converts the char array to string array

				// Makes sure input is only 6 chars (symbols arent counted)
				if (!(hexValue.Length == 6))
				{
					Console.WriteLine("Please make sure the hex value is 6 characters long (symbols aren't counted).");
					continue;
				}

				// Error checking
				for (int i = 0; i < 6; i++)
				{
					// if not an int and its a letter in the array
					if (int.TryParse(hexValue[i].ToString(), out int value) == false)
					{
						// Checks to see if only letters A-F are used
						if (!letters.Any(hexValue[i].ToString().Contains))
						{
							Console.WriteLine("Letters can only be between A-F");
							flag = false;
							break;
						}
					}
				}
				if (flag == true) { break; }
			}

			// Converts hex to rgb and inverts both
			for (int i = 0; i < 3; i += 1)
			{
				// f=15^0 e=14^1 d=13^2 c=12^3 b=11^4 a=10^5 9^6
				rgbValue[i] = Convert.ToInt32(hexValue[counter] + hexValue[counter + 1], 16);
				rgbInverted[i] = 255 - rgbValue[i]; // Invertes rgb value
				hexInverted[i] = rgbInverted[i].ToString("X").ToLower();
				if (hexValue[counter] == "f") { hexInverted[i] = "0" + hexInverted[i]; }
				counter += 2;
			}
			// Outputs results
			Console.WriteLine("\nHex to RGB:");
			Console.WriteLine("#{0}{1}{2}{3}{4}{5} : ({6}, {7}, {8})", hexValue[0], hexValue[1], hexValue[2], hexValue[3], hexValue[4], hexValue[5],
				rgbValue[0], rgbValue[1], rgbValue[2]);

			Console.WriteLine("\nInverted Value:");
			Console.WriteLine("#{0}{1}{2} : ({3}, {4}, {5})", hexInverted[0], hexInverted[1], hexInverted[2],
				rgbInverted[0], rgbInverted[1], rgbInverted[2]);

			Console.WriteLine("\nMonochrome RGB value:");
			// Converts rgb value to black or white and prints to console
			if ((rgbValue[0] * 0.2126) + (rgbValue[1] * 0.7152) + (rgbValue[2] * 0.0722) < 128) { Console.WriteLine("(0, 0, 0)"); }
			else { Console.WriteLine("(255, 255, 255)\n"); }

			Console.WriteLine("Press c to continue or e to exit");

			// Asks if user wants to continue using program
			while (true)
			{
				var key = Console.ReadKey();
				Console.WriteLine(""); // Tidy
				// Causes program to repeat
				if (key.KeyChar.ToString() == "c")
				{
					Console.Clear();
					break;
				}
				// Causes program to terminate
				else if (key.KeyChar.ToString() == "e")
				{
					cont = false;
					break;
				}
				// Repeats this loop with invalid input
				else { Console.WriteLine("Invalid key press, please try again"); }
			}
		}
	}
}
