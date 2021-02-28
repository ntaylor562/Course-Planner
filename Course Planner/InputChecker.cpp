#include "InputChecker.h"

#include <iostream>
#include <set>
#include <exception>

//Used to get an integer from user input in the console
//If user's input breaks the input stream, we output errorMessage and try getting input again until the user enters a valid input
int InputChecker::getInt(std::string errorMessage) {
	int input;
	std::cin >> input;
	while (!std::cin) {
		std::cout << errorMessage;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Used to get a double from user input in the console
//If user's input breaks the input stream, we output errorMessage and try getting input again until the user enters a valid input
double InputChecker::getDouble(std::string errorMessage) {
	double input;
	std::cin >> input;
	while (!std::cin) {
		std::cout << errorMessage;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Used to get an integer within a specified range, starting at start and ending at end (inclusive), from the console
//If user's input breaks the input stream or input is out of range, we output errorMessage and try getting input 
//again until the user enters a valid input.
int InputChecker::getIntRange(int start, int end, std::string errorMessage) throw(std::range_error) {
	if (start > end) throw std::range_error("Start point of range is greater than end point. Invalid range.");
	int input;
	std::cin >> input;
	while (!std::cin || input < start || input > end) {
		std::cout << errorMessage;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Used to get a double within a specified range, starting at start and ending at end (inclusive), from the console
//If user's input breaks the input stream or input is out of range, we output errorMessage and try getting input
//again until the user enters a valid input.
double InputChecker::getDoubleRange(double start, double end, std::string errorMessage) throw(std::range_error) {
	if (start > end) throw std::range_error("Start point of range is greater than end point. Invalid range.");
	int input;
	std::cin >> input;
	while (!std::cin || input < start || input > end) {
		std::cout << errorMessage;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Returns a string that the user enters into the console
//Equivalent to using std::cin >> str
std::string InputChecker::getString() {
	std::string input;
	std::cin >> input;
	return input;
}

//Returns a boolean value that user enters into the console
bool InputChecker::getBool(std::string errorMessage) {
	std::set<std::string> yesList;
	yesList.insert("y");
	yesList.insert("yes");
	yesList.insert("1");
	yesList.insert("true");
	yesList.insert("affirmative");
	yesList.insert("yeehaw");
	yesList.insert("sure");
	yesList.insert("yessir");

	std::set<std::string> noList;
	noList.insert("n");
	noList.insert("no");
	noList.insert("0");
	noList.insert("false");
	noList.insert("nah");
	noList.insert("nope");

	std::string input;
	std::cin >> input;
	for (char &x : input)
		x = std::tolower(x);
	
	
	while(1) {
		if (yesList.find(input) != yesList.end()) {
			std::cin.ignore(1000000, '\n');
			return true;
		}
		else if (noList.find(input) != noList.end()) {
			std::cin.ignore(1000000, '\n');
			return false;
		}

		if (yesList.find(input) == yesList.end() && noList.find(input) == noList.end()) {
			std::cin.ignore(1000000, '\n');
			std::cout << errorMessage;
			std::cin >> input;
			for (char &x : input)
				x = std::tolower(x);
		}
	}
	
}
