#ifndef Menu_H
#define Menu_H

#define dataFile "course_data.txt"

#include <vector>
#include <string>
#include <fstream>

class CourseModule;

class Menu {
private:
	std::ifstream inFile;
	std::ofstream outFile;

	std::vector<CourseModule*> Courses; //Container holding all the user's courses.
	CourseModule* addCourse(std::string sub, int num); //Checks if course is in list of courses then adds it if it isn't. This is the ONLY way you should be creating courses. 
	CourseModule* addCourse(std::string str);
	CourseModule* addCourse(CourseModule* course);
	void addMultiplePrereqs(CourseModule* course, const std::vector<CourseModule*> &prereqs);
	void removeCourse(std::vector<CourseModule*>::iterator courseIt); //method to remove a course from the list of courses DOESN'T check if course exists

	void readData(); //Reads data from dataFile file and populates the list of courses from that data
	void readPrerequisites(CourseModule* course, std::string str); //Enters all courses (format ex: CECS 100) from a string into a course's prerequisites. Does not check validity

	void saveData(); //Prints all the course data into the dataFile file so that it can be read next time program is run
	
	int readIntInput(); //Used in switch for user choice to select menu options and returns -1 if it's invalid (which should be caught by default case of switch).

	void mainMenu();
	void subMenuCourseEdit(); //Menu giving you options of which property of the course you want to edit.
	void subMenuCourseAdd(); //Menu first telling you to enter subject and number (ex: CECS 228) then gives you options to add other things like units
	void subMenuCourseRemove(); //Small menu asking which course to remove then removing and returns user to main menu
	//other menues are going to be of the form: subMenuName so for example, edit menu for a course is going to be subMenuCourseEdit()

	bool hasOneSpace(std::string str); //Checks if the input has multiple spaces. Used in inputValidCourse
	bool allInts(std::string str); //Checks if course number portion of input is only integers
	int enterInteger(); //Asks to input an integer then retries if the input is not valid.
	std::string inputValidCourse(std::string message = ""); //Asks to input a course then retries if the course entered is invalid
	std::vector<CourseModule*> inputValidCourses(std::string message = ""); //Same as inputValidCourse but it reads a line of courses and returns a container holding pointers to all the entered courses
	std::string inputNewValidCourse(std::string message = ""); //Same as inputValidCourse but it does not check if the course entered already exists. This is used to add a course.
	std::vector<CourseModule*> inputNewValidCourses(std::string message = ""); //Same as inputValidCourses but it does not check if the course entered already exists. This is used to add a course.

public:
	Menu ();
	void runMenu();
	CourseModule* courseSearch(std::string sub, int num); //TODO Improve this search function because it's just a linear search right now which is trash
	CourseModule* courseSearch(std::string str);
	std::vector<CourseModule*>::iterator courseSearchIt(std::string sub, int num); //TODO Improve this search function because it's just a linear search right now which is trash
	std::vector<CourseModule*>::iterator courseSearchIt(std::string str);

	void printAllCourses();
	void printAllIncompleteDataCourses();
	void printAllCourseData();
	std::string fullCourseInfo(CourseModule *course);

};

#endif