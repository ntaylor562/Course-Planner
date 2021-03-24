#ifndef Menu_H
#define Menu_H

#define settingsConfig "settings.txt"
#define scheduleDataFile "schedule_preferences.txt"

#include <vector>
#include <string>
#include <fstream>
#include "CourseModule.h"
#include "CourseGraph.h"
#include "Scheduler.h"
#include "Major.h"

/**
 * @brief Temporary class to drive the program giving the option to add, remove, and edit courses in a list
 * @details Meant to be replaced in the future with an actual object oriented design and GUI
 * @details I started this project when I didn't know much, that's why it's like this
*/
class Menu {
protected:
	/**
	 * @brief Name of the file that we save data to when using save feature
	*/
	std::string courseDataFileName;

	bool autoSave;

	/**
	 * @brief Container holding all the user's courses that have been added.
	*/
	CourseGraph Courses;

	/**
	 * @brief Handles generating schedules
	*/
	Scheduler CourseScheduler;

	/**
	 * @brief List of majors to choose from
	*/
	std::list<Major*> majorList;
	
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
	 * @details If the course entered does not exist, we create it
	 * @return Pointer to a course entered by the user (new or existing)
	*/
	CourseModule *inputNewValidCourse();

	/**
	 * @brief Asks to input a season and a year (Ex: Spring 2020) and retries if input is invalid
	 * @param message The message you want to display asking for input
	 * @return Empty semester entered by the user containing only the season and the year
	*/
	Semester enterSemester(std::string message = "Enter a semester (Ex: Spring 2020): ") const;

public:
	/**
	 * @brief Constructor to read data from a file to populate the list of courses
	*/
	Menu ();

	/**
	 * @brief Helps the user set up the courses they need to take by letting them select a major and their electives. 
	*/
	void setupMenu();

	/**
	 * @brief Prints a welcome message then runs the main menu
	*/
	void runMenu();
	
	/**
	 * @brief Gives a list of options and asks for user to select one then performs that action when entered
	*/
	void mainMenu();
	
	/**
	 * @brief Allows user to add a course by entering the course subject and number
	 * @details Allows user to also enter information for the course before finishing
	*/
	vertex *subMenuCourseAdd();

	/**
	 * @brief Asks user which course to remove then removes the course specified
	*/
	void subMenuCourseRemove();

	/**
	 * @brief Allows the user to edit a course's information (not including course subject and number)
	*/
	void subMenuCourseEdit();

	/**
	 * @brief Searches the list of courses for 
	*/
	void subMenuCourseSearch();

	/**
	 * @brief Allows user to generate a schedule based on the courses entered
	 * @details Use can disallow semesters, 
	*/
	void subMenuCourseScheduler();

	/**
	 * @brief Allows user to configure program settings like auto save, reset courses, and change the course data file name
	*/
	void subMenuSettings();

	/**
	 * @brief Allows user to add, edit, or remove majors which are used to prefill their courses
	*/
	void subMenuMajorCreator();

	/**
	 * @brief Uses a Menu object to let user add, edit, and remove courses from their major
	*/
	void subMenuAddMajor();

	/**
	 * @brief Allows user to add a restriction to the course scheduler
	 * @details User can restrict semesters, individual semesters, or courses for an individual semester
	*/
	void manageRestrictions();

	/**
	 * @brief Gives user a menu to edit a course allowing them to change different attributes
	 * @param c Course being edited
	*/
	void editCourse(CourseModule &c);

	/**
	 * @brief Gives user a menu to edit a major allowing them to change different attributes
	 * @param m Major being edited
	*/
	void editMajor(Major &m);

	/**
	 * @brief Whenever we update the course graph, we should update the scheduler because the scheduler would have an old data set to work with
	*/
	void updateScheduler();

	/**
	 * @brief Loads settings from the settings.txt file
	*/
	void loadSettings();

	/**
	 * @brief Saves settings to the settings.txt file
	*/
	void saveSettings();

	/**
	 * @brief Loads data on majors for the user to pick courses from
	*/
	void loadMajors();

	/**
	 * @brief Saves a list of majors so we know which majors to load when accessing them
	*/
	void saveMajors();
	
	/**
	 * @brief Enters all courses (format ex: CECS 100) from a string into a course's prerequisites. Does not check validity
	 * @param course Course we're adding prerequisites to
	 * @param str String we're reading courses from
	*/
	void readPrerequisites(CourseModule &course, std::string str);

	/**
	 * @brief Adds multiple courses to the list of prerequisites of a course
	 * @param course The course receiving the list of prerequisites
	 * @param prereqs Vector containing courses that are prerequisites for course
	*/
	void addMultiplePrereqs(CourseModule *course, const std::vector<CourseModule *> &prereqs);

	/**
	 * @brief Removes a vertex and all courses that are prerequisites of v from the graph and removes their prerequisites as well
	 * @details Used when marking a course as complete.
	 * @param g Graph we're removing courses from
	 * @param v Vertex whose self and prerequisites are getting deleted
	*/
	void deletePrereqs(CourseGraph &g, vertex &v) const;

	/**
	 * @brief Prints all courses' subjects and numbers in the given graph
	 * @param g Graph whose courses we're printing
	 * @param title What we want to print before printing the courses
	*/
	void printAllCourses(const CourseGraph &g, std::string title = "Current list of courses:") const;

	/**
	 * @brief Prints all the courses' subjects and numbers that have incomplete data
	 * @param g Graph whose courses we're printing
	*/
	void printAllIncompleteDataCourses(const CourseGraph &g) const;

	/**
	 * @brief Prints all data for each course in the list of courses
	 * @param g Graph whose courses we're printing
	*/
	void printAllCourseData(const CourseGraph &g);

	/**
	 * @brief Prints all restrictions on the schedule generator
	*/
	void printScheduleRestrictions() const;

	/**
	 * @brief Prints the schedule generated by the course scheduler 
	 * @param s Semester we're starting our schedule from
	*/
	void printSchedule(Semester s) const;

	/**
	 * @brief Prints all courses that are not in the list of taken courses or their prerequisites
	 * @param g Graph with all the courses
	 * @param taken List containing all the courses that have already been taken
	*/
	void printRemainingCourses(const CourseGraph &g, std::list<CourseModule> taken);

	/**
	 * @brief Prints all courses in the list of taken courses as well as their prerequisites
	 * @param g Graph with all the courses
	 * @param taken List containing all the courses taht have already been taken
	*/
	void printCompletedCourses(const CourseGraph &g, std::list<CourseModule> taken);

	/**
	 * @brief Gets all course data as a string
	 * @param course Course we're getting data from
	 * @return String containing all data for the specified course
	*/
	std::string fullCourseInfo(const CourseModule &course);

};

#endif