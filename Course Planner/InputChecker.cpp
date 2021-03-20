#include "InputChecker.h"

#include <iostream>
#include <set>
#include <exception>

//Used to get an integer from user input in the console
//If user's input breaks the input stream, we output message and try getting input again until the user enters a valid input
int InputChecker::getInt(std::string message) {
	int input;
	std::cin >> input;
	while (!std::cin) {
		std::cout << message;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Used to get a double from user input in the console
//If user's input breaks the input stream, we output message and try getting input again until the user enters a valid input
double InputChecker::getDouble(std::string message) {
	double input;
	std::cin >> input;
	while (!std::cin) {
		std::cout << message;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Used to get an integer within a specified range, starting at begin and ending at end (inclusive), from the console
//If user's input breaks the input stream or input is out of range, we output message and try getting input 
//again until the user enters a valid input.
int InputChecker::getIntRange(int begin, int end, std::string message) throw(std::range_error) {
	if (begin > end) throw std::range_error("Start point of range is greater than end point. Invalid range.");
	int input;
	std::cin >> input;
	while (!std::cin || input < begin || input > end) {
		std::cout << message;
		std::cin.clear();
		std::cin.ignore(1000000, '\n');
		std::cin >> input;
	}
	std::cin.ignore(1000000, '\n');
	return input;
}

//Used to get a double within a specified range, starting at begin and ending at end (inclusive), from the console
//If user's input breaks the input stream or input is out of range, we output message and try getting input
//again until the user enters a valid input.
double InputChecker::getDoubleRange(double begin, double end, std::string message) throw(std::range_error) {
	if (begin > end) throw std::range_error("Start point of range is greater than end point. Invalid range.");
	int input;
	std::cin >> input;
	while (!std::cin || input < begin || input > end) {
		std::cout << message;
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

std::string InputChecker::getLine() {
	std::string output;
	std::getline(std::cin, output);
	return output;
}

std::string InputChecker::trim(std::string str) {
	str = str.substr(str.find_first_not_of(" "));
	str = str.substr(0, str.find_last_not_of(" ") + 1);
	return str;
}

//Returns a boolean value that user enters into the console
bool InputChecker::getBool(std::string message) {
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
			std::cout << message;
			std::cin >> input;
			for (char &x : input)
				x = std::tolower(x);
		}
	}
	
}
