#ifndef COURSE_MODULE_H
#define COURSE_MODULE_H
#include <vector>
#include <string>
#include <iostream>


/**
 * @brief Represents a course which has a course subject, course number, course title, number of units, description, and can have prerequisites
*/
class CourseModule {
private:

	/**
	 * @brief Vector containing pointers to the prerequisites for this course
	*/
	std::vector<CourseModule*> prerequisites;

	/**
	 * @brief Vector containing pointers to the courses this course is a prerequisite for
	 * @details This should be temporary because I'm planning on using a graph in the future to represent the courses
	*/
	std::vector<CourseModule*> prerequisiteFor;
	
	int units, courseNumber;

	/**
	 * @brief The acronym for the course subject
	 * @details Example: CECS, BIO, CS, ENGR, etc.
	*/
	std::string courseSubject;

	/**
	 * @brief Course title (Ex: Computer Architecture and Organization)
	*/
	std::string courseTitle = "";

	/**
	 * @brief Course description (Ex: Review of logic design. Instruction set architecture. Arithmetic logic...)
	*/
	std::string courseDescription = "";

	/**
	 * @brief Adds a course to the list of courses this course is a prerequisite for
	 * @param course A pointer to the course we're adding to this course's prerequisites
	*/
	void addPrerequisiteFor(CourseModule* course);

	/**
	 * @brief Removes a course from the list of courses this course is a prerequisite for
	 * @param course The course whose prerequisites we're removing this course from
	*/
	void removePrerequisiteFor(CourseModule* course);

public:

	/**
	 * @brief Initializes course's member variables
	 * @details Subject and number are required so that the course can be identifiable
	 * @param subject Course subject (Ex: CECS)
	 * @param number Course number (Ex: 341)
	 * @param title Course title (Ex: Computer Architecture and Organization)
	 * @param numOfUnits Course units (Ex: 3)
	 * @param description Course description (Ex: Review of logic design. Instruction set architecture. Arithmetic logic...)
	 * @param prereqs Vector containing a list of pointers to courses this course has as prerequisites
	*/
	CourseModule(std::string subject, int number, std::string title = "", int numOfUnits = 0, std::string description = "", std::vector<CourseModule *> prereqs = std::vector<CourseModule*>());
	
	/**
	 * @brief Destructor that goes through the list of courses this course is a prerequisite for and removes this course from their prerequisites
	*/
	~CourseModule();

	//Self explanatory functions

	void setCourseSubject(std::string str);
	void setCourseNumber(int num);
	void setCourseTitle(std::string str);
	void setUnits(int num);
	void setDescription(std::string);

	std::string getCourseSubject() const;
	int getCourseNumber() const;
	std::string getCourseTitle() const;
	int getUnits() const;
	std::string getDescription() const;

	/**
	 * @return String representing this course's subject, course number, and course title
	*/
	std::string getCourseName() const;


	/**
	 * @brief Puts the prerequisite courses for this course into a string including only the course subject and course number
	 * @return String containing course subjects and numbers of this course's prerequisites separated by spaces. Returns empty string if there are no prerequisites
	*/
	std::string getPrereqsAsString() const;

	/**
	 * @brief Adds a prerequisite to this course
	 * @param course Pointer to the course we're adding as a prerequisite
	*/
	void addPrerequisite(CourseModule* course);

	/**
	 * @brief Removes a course from this course's prerequisites
	 * @details Does nothing if the course passed in is not a prerequisite
	 * @param course Prerequisite course we're removing
	*/
	void removePrerequisite(CourseModule* course);

	/**
	 * @brief Gets the list of prerequisites for this course
	 * @returns a pointer to the list of prerequisites to save memory
	 * @return Pointer to a vector containing a list of pointers to courses this course has as prerequisites
	*/
	std::vector<CourseModule*>* getPrerequisites();

	/**
	 * @brief Gets the list of courses this course is a prerequisite for
	 * @details returns a pointer to the list of prerequisites to save memory
	 * @return Pointer to a vector containing a list of pointers to courses this course is a prerequisite for
	*/
	std::vector<CourseModule *> *getPrerequisiteFor();

	/**
	 * @brief Checks whether all fields have been filled out for this course
	 * @return True if the course subject, number, units, title, and description all have values, false otherwise
	*/
	bool dataComplete() const;

	/**
	 * @brief Overloaded << operator to more easily print a course
	 * @details Inserts the course subject and course number into the ostream
	 * @param os Ostream variable we're inserting into
	 * @param course Course we want to output
	 * @return The ostream variable passed in
	*/
	friend std::ostream &operator<<(std::ostream &os, const CourseModule &course);
};

#endif
