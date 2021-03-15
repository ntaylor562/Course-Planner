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
	CourseModule(std::string subject, int number, std::string title = "", int numOfUnits = NULL, std::string description = "");

	/**
	 * @brief Constructor to initialize the course subject and course number
	 * @details If string entered is not a valid course, we return 
	 * @param course String containing the course's subject and course number
	*/
	CourseModule(std::string course);

	CourseModule(const CourseModule &c);

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
	 * @brief Checks whether all fields have been filled out for this course
	 * @return True if the course subject, number, units, title, and description all have values, false otherwise
	*/
	bool dataComplete() const;

	/**
	 * @return A string showing the course subject and course number
	*/
	std::string ToString() const;

	/**
	 * @brief Overloaded << operator to more easily print a course
	 * @details Inserts the course subject and course number into the ostream
	 * @param os Ostream variable we're inserting into
	 * @param course Course we want to output
	 * @return The ostream variable passed in
	*/
	friend std::ostream &operator<<(std::ostream &os, const CourseModule &course);

	/**
	 * @brief Used to determine if two course names are the same (Ex. CECS 100 == CECS 100 is true)
	 * @details ONLY checks course subject and course number are the same
	 * @param c Course we're comparing to
	 * @return True if course subject and course number are matching, false otherwise
	*/
	bool operator==(const CourseModule &c) const;

	/**
	 * @brief First compares subject then compares course number
	 * @param c Course being compared
	 * @return Compares strings of this course and course c, then compares course numbers if subjects are equal
	*/
	bool operator<(const CourseModule &c) const;

	/**
	 * @brief First compares subject then compares course number
	 * @param c Course being compared
	 * @return Compares strings of this course and course c, then compares course numbers if subjects are equal
	*/
	bool operator>(const CourseModule &c) const;

};

#endif
