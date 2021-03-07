#include <iostream>
#include <string>
#include "CourseInputChecker.h"

std::string CourseInputChecker::getCourses(std::string message) {
	std::string input;
	std::getline(std::cin, input);

	std::string result = "";

	do {
		try {
			if (input == "0" || input == "") return "";
			input = input.substr(input.find_first_not_of(' '), std::string::npos);

			if (input.find(' ') == std::string::npos) throw std::invalid_argument("Input not formatted correctly."); //Input contains no spaces

			if ('0' <= input[0] && input[0] <= '9') throw std::invalid_argument("Subject cannot start with a number.");

			std::string sub = input.substr(0, input.find_first_of(' '));
			input = input.substr(input.find_first_of(' '), std::string::npos);
			input = input.substr(input.find_first_not_of(' '), std::string::npos);

			if ('0' > input[0] || input[0] > '9') { //The subject has a space in it (Ex: E E 381)
				if (input.find_first_of(' ') == std::string::npos) throw std::invalid_argument("No course number entered");
				sub += " " + input.substr(0, input.find_first_of(' '));
				input = input.substr(input.find_first_of(' '), std::string::npos);
				input = input.substr(input.find_first_not_of(' '), std::string::npos);
			}

			if ('0' > input[0] || input[0] > '9') throw std::invalid_argument("Invalid number"); //A number was not inputted after the course subject

			if (input.find_first_not_of("1234567890") == std::string::npos) { //Input is a good number and last in the list
				result += sub + " " + input;
				input = "";
			}
			else if (input[input.find_first_not_of("1234567890")] == ' ') { //Input is good and there's more in the list
				result += sub + " " + input.substr(0, input.find_first_of(' ')) + " ";
				input = input.substr(input.find_first_of(' ') + 1, std::string::npos);
			}
			else throw std::invalid_argument("Invalid number.");

		}
		catch (std::invalid_argument ex) {
			result = "";
			std::cout << message << ((message == "") ? "" : " ") << ex.what() << " Try again: ";
			std::getline(std::cin, input);
		}
	} while (input != "");

	return result;
}

bool CourseInputChecker::isValidCourse(std::string input) {
	if (input == "0" || input == "") return "";
	input = input.substr(input.find_first_not_of(' '), std::string::npos);

	if (input.find(' ') == std::string::npos) return false; //Input contains no spaces

	if ('0' <= input[0] && input[0] <= '9') return false; //Subject can't start with a number

	std::string sub = input.substr(0, input.find_first_of(' '));
	input = input.substr(input.find_first_of(' '), std::string::npos);
	input = input.substr(input.find_first_not_of(' '), std::string::npos);

	if ('0' > input[0] || input[0] > '9') { //The subject has a space in it (Ex: E E 381)
		if (input.find_first_of(' ') == std::string::npos) return false; //No course number entered
		sub += " " + input.substr(0, input.find_first_of(' '));
		input = input.substr(input.find_first_of(' '), std::string::npos);
		input = input.substr(input.find_first_not_of(' '), std::string::npos);
	}

	if ('0' > input[0] || input[0] > '9') return false; //A number was not inputted after the course subject

	if (input.find_first_not_of("1234567890") == std::string::npos) { //Input is a good number and last in the list
		return true;
	}
	else if (input[input.find_first_not_of("1234567890")] == ' ') { //Input is good and there's more in the list
		return true;
	}
	else return false; //Invalid number
}
