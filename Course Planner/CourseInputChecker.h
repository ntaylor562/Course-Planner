#ifndef COURSE_INPUT_CHECKER_H
#define COURSE_INPUT_CHECKER_H

#include <string>
#include "CourseModule.h"
#include "InputChecker.h"

/**
 * @brief Used to get input specifically for courses
*/
class CourseInputChecker : InputChecker {
public:
	/** 
	 * @brief Asks for input until user inputs one or more valid courses, repeating when input is invalid\n
	 * @details Example: "CECS 228 CECS 123" is a valid input while "123 blah" is not a valid input
	 * @details test test test
	 * @param message Option to provide an error message specified by message parameter
	 * @return String easy to parse into a list of courses
	*/
	static std::string getCourses(std::string message = "");

	/**
	 * @brief Used to check if a string is a valid input for a course, including the course subject and the course number
	 * @param input String we'll validate is a course
	 * @return True if input is a valid course (Ex. "CECS 100")
	*/
	static bool isValidCourse(std::string input);
};

#endif