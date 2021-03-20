#ifndef INPUT_CHECKER_H
#define INPUT_CHECKER_H

#include <string>

/**
 * @brief Used to error check and get user's input from std::cin
 * @details If input is invalid (a character in an integer, double is out of range, etc.), we repeat until we get valid input
 * Programmer has the option to include an error message when input is invalid
*/
class InputChecker {
public:
	/**
	 * @brief Gets an integer from user input in the console
	 * @param message Error message
	 * @return int entered by user
	*/
	static int getInt(std::string message = "");

	/**
	 * @brief Gets a double from user input in the console
	 * @param message Error message
	 * @return double entered by user
	*/
	static double getDouble(std::string message = "");

	/**
	 * @brief Gets an integer within the range [begin, end] from user input in the console
	 * @param begin Start of range
	 * @param end End of range
	 * @param message Error message
	 * @return int entered by user
	*/
	static int getIntRange(int begin, int end, std::string message = "");

	/**
	 * @brief Gets a double within the range [begin, end] from user input in the console
	 * @param begin Start of range
	 * @param end End of range
	 * @param message Error message
	 * @return double entered by user
	*/
	static double getDoubleRange(double begin, double end, std::string message = "");
	
	//Gets a string. Equivalent to using std::cin >> str;

	/**
	 * @brief Gets a string. Equivalent to using std::cin str;
	 * @return std::string entered by user
	*/
	static std::string getString();

	/**
	 * @brief Gets a line of input from the user. Equivalent to using std::getline(std::cin, returnValue)
	 * @return Line of input entered by user
	*/
	static std::string getLine();

	//Gets a boolean value. User can enter a variety of inputs such as T, false, no, Yes, etc.
	//Allows for a custom error message

	/**
	 * @brief Gets a boolean value. User can enter a variety of inputs such as T, false, no, Yes, etc.
	 * @param message Error message
	 * @return True or false, depending on the user's input
	*/
	static bool getBool(std::string message = "");

};

#endif