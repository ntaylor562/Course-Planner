#ifndef INPUT_CHECKER_H
#define INPUT_CHECKER_H

#include <string>

/*
* Used to get integers, doubles, strings, and boolean values from the common input
* If an input is invalid (a character in an integer, double is out of range, etc.), we keep trying to get a valid input
* Programmer has the option of providing an error message when input is invalid
*/
class InputChecker {
public:

	//Gets an integer. Allows for a custom error message
	static int getInt(std::string = "");

	//Gets a double. Allows for a custom error message
	static double getDouble(std::string = "");

	//Gets an integer within a specified range. Allows for a custom error message
	static int getIntRange(int, int, std::string = "");

	//Gets a double within a specified range. Allows for a custom error message
	static double getDoubleRange(double, double, std::string = "");
	
	//Gets a string. Equivalent to using std::cin >> str;
	static std::string getString();

	//Gets a boolean value. User can enter a variety of inputs such as T, false, no, Yes, etc.
	//Allows for a custom error message
	static bool getBool(std::string = "");

};

#endif