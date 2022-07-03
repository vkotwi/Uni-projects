using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;


namespace PrimeMinisterGame
{
	class Program
	{
		static void Main()
		{
			bool replay;

			while (true)
			{
				// Set to true so loop will always run unless changed
				replay = true;
				Console.WriteLine("\nWhich game would like like to play?" +
					"\n1. Which prime minister served first?" +
					"\n2. Which prime minister served on this date?" +
					"\n3. Exit.");
				Console.Write(">> ");
				// Makes sure user input is integer and between 1 an 3
				if (int.TryParse(Console.ReadLine(), out int answer) && Enumerable.Range(1, 3).Contains(answer))
				{
					if (answer == 1)
					{
						// Creates new instance of class game
						Game newGuessPMGame = new Game();
						while (replay)
						{
							// Runs pm guessing game
							replay = newGuessPMGame.InitFirstPMGame();
						}
					}
					else if (answer == 2)
					{
						// Creates new instance of class game
						Game newPMDateGame = new Game();
						while (replay)
						{
							// Runs the date guessing game
							replay = newPMDateGame.InitPMDateGame();
						}
					}
					else { break; } // Answers can only be between 1 and 3 so 3rd option will always be exit
				}
				else { Console.WriteLine("Incorrect input, please enter 1, 2 or 3.\n"); }
			}

			Console.WriteLine("Thanks for playing.");
			Console.ReadKey();
		}
	}

	// Actual games
	class Game
	{
		// Initiates pm guessing game, loading primister info into a list
		public bool InitFirstPMGame()
		{
			string checkReplay;
			int runs = 0;
			bool flag;
			List<List<string>> listOfPMs = GetData();
			List<List<string>> chosenPMs = new List<List<string>>();

			Random rnd = new Random();

			Player score = new Player();
			score.TotalScore = 0;

			while (runs != 5)
			{

				// Get 3 random MPs from list to use
				while (true)
				{
					// Cleans up list for ne
					chosenPMs.Clear();
					flag = true; // Resets flag

					chosenPMs.Add(listOfPMs[rnd.Next(listOfPMs.Count)]);
					chosenPMs.Add(listOfPMs[rnd.Next(listOfPMs.Count)]);
					chosenPMs.Add(listOfPMs[rnd.Next(listOfPMs.Count)]);

					try
					{
						CheckPMDups(chosenPMs);
					}
					catch (SameStringValueException)
					{
						flag = false;
					}
					// C# needs try-catch-else
					if (flag == true) { break; }
				}

				Console.WriteLine("\nWhich one of these primisters served first?\n" +
					"Please enter 1, 2 or 3.");
				for (int i = 0; i < chosenPMs.Count; i++)
				{
					Console.WriteLine("{0}. {1}", i + 1, chosenPMs[i][1]);
				}

				while (true)
				{
					Console.Write(">> ");
					// Checks if answer is in correct format (int)
					if (int.TryParse(Console.ReadLine(), out int answer) && Enumerable.Range(1, 3).Contains(answer)) // Use range instead? TODO
					{
						// Don't know answer so checks against all 3 possible ones
						if (int.Parse(chosenPMs[answer - 1][0]) <= int.Parse(chosenPMs[0][0]) && int.Parse(chosenPMs[answer - 1][0]) <= int.Parse(chosenPMs[1][0]) && int.Parse(chosenPMs[answer - 1][0]) <= int.Parse(chosenPMs[2][0]))
						{
							score.TotalScore += 1;
							Console.WriteLine("Correct! Your current score is {0} out of 5", score.TotalScore);
						}
						else { Console.WriteLine("Incorrect! Your current score is {0} out of 5", score.TotalScore); }
						break;
					}
					else
					{
						Console.WriteLine("Please only enter one number, 1, 2 or 3.\n");
					}
				}

				// Emptys for next use
				chosenPMs.Clear();
				runs++;
			}

			Console.WriteLine("\nYou scored {0} out of 5\n", score.TotalScore);

			Console.WriteLine("Would you like to play again? Y/N");
			while (true)
			{
				Console.Write(">> ");
				checkReplay = Console.ReadLine();
				if (checkReplay == "Y" || checkReplay == "y") { return true; }
				else if (checkReplay == "N" || checkReplay == "n") { return false; }
				else { Console.WriteLine("Please enter either 'Y' or 'N' without the '"); }
			}
		}

		// Initiates pm date guessing game, loading primister info into a list
		public bool InitPMDateGame()
		{
			string checkReplay;
			string date;
			int pmAnswer;
			int runs = 0;
			bool flag;
			List<List<string>> listOfPMs = GetData();
			List<List<string>> chosenPMs = new List<List<string>>();

			Random rnd = new Random();

			Player score = new Player();
			score.TotalScore = 0;

			while (runs != 5)
			{

				// Get 3 random MPs from list to use
				while (true)
				{
					// Cleans up lists for next use
					chosenPMs.Clear();

					flag = true; // Resets flag

					chosenPMs.Add(listOfPMs[rnd.Next(listOfPMs.Count)]);
					chosenPMs.Add(listOfPMs[rnd.Next(listOfPMs.Count)]);
					chosenPMs.Add(listOfPMs[rnd.Next(listOfPMs.Count)]);

					try
					{
						CheckPMDups(chosenPMs);
					}
					catch (SameStringValueException)
					{
						flag = false;
					}
					// C# needs try-catch-else
					if (flag == true)
					{
						// Chooses random pm from list
						pmAnswer = rnd.Next(0, 3);
						// Gets a date to use for game
						date = chosenPMs[pmAnswer][3];
						break;
					}
				}

				Console.WriteLine("\nWhich prime minister serving on {0}? " +
					"Please enter 1, 2 or 3.", date);
				for (int i = 0; i < chosenPMs.Count; i++)
				{
					Console.WriteLine("{0}. {1}", i + 1, chosenPMs[i][1]);
				}

				while (true)
				{
					Console.Write(">> ");
					// Checks if answer is in correct format (int)
					if (int.TryParse(Console.ReadLine(), out int answer) && Enumerable.Range(1,3).Contains(answer)) // Use range instead? TODO
					{
						// Checks if answer is correct
						Console.WriteLine("{0}", chosenPMs[answer-1][3]);


						if (chosenPMs[answer-1][3].ToString() == date)
						{
							score.TotalScore += 1;
							Console.WriteLine("Correct! Your current score is {0} out of 5", score.TotalScore);
						}
						else { Console.WriteLine("Incorrect! Your current score is {0} out of 5", score.TotalScore); }
						break;
					}
					else
					{
						Console.WriteLine("Please only enter one number, 1, 2 or 3.\n");
					}
				}

				// Emptys for next use
				chosenPMs.Clear();
				runs++;
			}

			//Displays final score and asks if they want to play again
			Console.WriteLine("\nYou scored {0} out of 5\n", score.TotalScore);
			Console.WriteLine("Would you like to play again? Y/N");
			while (true)
			{
				Console.Write(">> ");
				checkReplay = Console.ReadLine();
				if (checkReplay == "Y" || checkReplay == "y") { return true; }
				else if (checkReplay == "N" || checkReplay == "n") { return false; }
				else { Console.WriteLine("Please enter either 'Y' or 'N' without the '"); }
			}
		}
		

		// Gets priminster data from csv file
		List<List<string>> GetData()
		{
			// List that stores lists of prime ministers and data
			List<List<string>> listOfPMs = new List<List<string>>();

			// Placeholder list
			List<string> srcLine = new List<string>();

			// Gets program's current path and looks for a file called "".csv to import
			string path = @"list_of_prime_ministers_of_uk-1-UPDATE.csv";
			bool flag = false;

			while (true)
			{
				try
				{
					// Uses StreamReader to get each line from csv, seperating each line into a list by the ',', and adding that list into one big list of lists
					using (StreamReader src = File.OpenText(path))
					{
						// Loops till nothing left to read
						while (!src.EndOfStream)
						{
							listOfPMs.Add(src.ReadLine().Split(',').ToList());
						}
					}
					flag = true;
				}
				catch (Exception)
				{
					// Allows user to add file o folder if not found
					Console.WriteLine("File could not be read. Please make sure the 'list_of_prime_ministers_of_uk-1-UPDATE.csv' is in the same folder as the game.");
					Console.WriteLine("Current dictionary: {0}", Directory.GetCurrentDirectory());
					Console.WriteLine("Once you've copied it there, press any key to try again.\n");
					Console.ReadKey();
				}
				if (flag == true) { break; }
			}
			//Removes headers from list
			listOfPMs.RemoveAt(0);
			return listOfPMs;
		}

		// Checks if any given strings have same value
		static void CheckPMDups(List<List<string>> PMsList)
		{
			// COunt unnedded as lists will always have same lengths, order doesn't matter
			if (PMsList[0][1].All(PMsList[1][1].Contains) || PMsList[0][1].All(PMsList[2][1].Contains) || PMsList[1][1].All(PMsList[2][1].Contains))
			{
				throw new SameStringValueException();
			}
		}
	}

	// Player score class
	public class Player
	{
		// Player's score
		public int TotalScore { get; set; }
	}

	// Exception class for showing error
	public class SameStringValueException : Exception
	{
		public SameStringValueException() : base
			("2 or more strings share the same value.") { }

	}
}