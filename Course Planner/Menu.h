#ifndef Menu_H
#define Menu_H

#define dataFile "course_data.txt"

#include <vector>
#include <string>
#include <fstream>
#include "CourseModule.h"
#include "CourseGraph.h"

/**
 * @brief Temporary class to drive the program giving the option to add, remove, and edit courses in a list
 * @details Meant to be replaced in the future with an actual object oriented design
 * @details I started this project when I didn't know much, that's why it's like this
*/
class Menu {
private:

	/**
	 * @brief Container holding all the user's courses that have been added.
	*/
	CourseGraph Courses;
	
	/**
	 * @brief Asks to input a course then retries if the course entered is invalid
	 * @return Pointer to the course entered. Returns nullptr if course entered does not exist in the list of courses
	*/
	CourseModule *inputValidCourse();

	/**
	 * @brief Reads a line of courses from user input and repeats when input is invalid
	 * @details If any of the courses entered do not exist in the list of courses, the course is added to the list of courses
	 * @return Vector containing all the courses entered
	*/
	std::vector<CourseModule*> inputValidCourses();

	/**
	 * @brief Asks to input a new course then retries if the course entered is invalid
	 * @return Pointer to a course entered by the user (new or existing)
	*/
	CourseModule *inputNewValidCourse();

public:
	/**
	 * @brief Constructor to read data from a file to populate the list of courses
	*/
	Menu ();


	//TODO Consider if this method is still relevant
	/**
	 * @brief Adds multiple courses to the list of prerequisites of a course
	 * @param course The course receiving the list of prerequisites
	 * @param prereqs Vector containing courses that are prerequisites for course
	*/
	void addMultiplePrereqs(CourseModule *course, const std::vector<CourseModule *> &prereqs);

	/**
	 * @brief Prints a welcome message then runs the main menu
	*/
	void runMenu();
	
	/**
	 * @brief Gives a list of options and asks for user to select one then performs that action when entered
	*/
	void mainMenu();

	/**
	 * @brief Allows the user to edit a course's information (not including course subject and number)
	*/
	void subMenuCourseEdit();
	
	/**
	 * @brief Allows user to add a course by entering the course subject and number
	 * @details Allows user to also enter information for the course before finishing
	*/
	void subMenuCourseAdd();

	/**
	 * @brief Asks user which course to remove then removes the course specified
	*/
	void subMenuCourseRemove();
	
	/**
	 * @brief Enters all courses (format ex: CECS 100) from a string into a course's prerequisites. Does not check validity
	 * @param course Course we're adding prerequisites to
	 * @param str String we're reading courses from
	*/
	void readPrerequisites(CourseModule *course, std::string str);

	/**
	 * @brief Prints all courses' subjects and numbers in the list of courses
	*/
	void printAllCourses();

	/**
	 * @brief Prints all the courses' subjects and numbers that have incomplete data
	*/
	void printAllIncompleteDataCourses();

	/**
	 * @brief Prints all data for each course in the list of courses
	*/
	void printAllCourseData();

	/**
	 * @brief Gets all course data as a string
	 * @param course Course we're getting data from
	 * @return String containing all data for the specified course
	*/
	std::string fullCourseInfo(const CourseModule &course);

};

#endif