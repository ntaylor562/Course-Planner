#ifndef COURSE_INPUT_CHECKER_H
#define COURSE_INPUT_CHECKER_H

#include <string>
#include "CourseModule.h"
#include "InputChecker.h"

class CourseInputChecker : InputChecker {
public:
	//Asks for input until the user inputs one or multiple valid courses, repeating when the input is invalid
	//Programmer has the option to include an error message
	//Example: "CECS 228 CECS 123" is a valid input while "123 blah" is not a valid input'
	//Returns a string easy to parse into a list of courses
	static std::string getCourses(std::string message = "");
};

#endif