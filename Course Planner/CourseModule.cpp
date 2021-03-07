#include <string>
#include <vector>
#include <iostream>//DEBUGGING

#include "CourseInputChecker.h"
#include "CourseModule.h"

//Constructor
CourseModule::CourseModule(std::string subject, int number, std::string title, int numOfUnits, std::string description, std::vector<CourseModule*> prereqs) :
	courseSubject(subject), courseNumber(number), courseTitle(title), units(numOfUnits), courseDescription(description) {}

//Constructor using string as initializer to make things easier
CourseModule::CourseModule(std::string course) {
	if (CourseInputChecker::isValidCourse(course)) {
		courseSubject = course.substr(0, course.find_first_of("0123456789") - 1);
		courseNumber = std::stoi(course.substr(course.find_first_of("0123456789"), std::string::npos));
		units = NULL;
		courseTitle = "";
		courseDescription = "";
	}
	else {
		courseSubject = "ERROR";
		courseNumber = 404;
		units = -1;
		courseTitle = "ERROR INITIALIZING COURSE";
		courseDescription = "STRING USED IN CONSTRUCTOR DOES NOT REPRESENT A VALID COURSE. ENTER COURSE LIKE \"CECS 100\"";
	}
}

CourseModule::CourseModule(const CourseModule &c) {
	courseSubject = c.courseSubject;
	courseNumber = c.courseNumber;
	units = c.units;
	courseTitle = c.courseTitle;
	courseDescription = c.courseDescription;
}

//Setters

void CourseModule::setCourseSubject(std::string str)	{ courseSubject = str; }

void CourseModule::setCourseNumber(int num)				{ courseNumber = (num >= 0) ? num : 0; }

void CourseModule::setCourseTitle(std::string str)		{ courseTitle = str; }

void CourseModule::setUnits(int num)					{ units = (num >= 0) ? num : 0; }

void CourseModule::setDescription(std::string str)		{ courseDescription = str; }


//Getters

std::string CourseModule::getCourseSubject() const		{ return courseSubject; }

int CourseModule::getCourseNumber() const				{ return courseNumber; }

std::string CourseModule::getCourseTitle() const		{ return courseTitle; }

int CourseModule::getUnits() const						{ return units; }

std::string CourseModule::getDescription() const		{ return courseDescription; }

//Returns a string containing the course's subject and number followed by the course title
std::string CourseModule::getCourseName() const {
	return courseSubject + " " + std::to_string(courseNumber) + " - " + courseTitle;
}

//Checks if any course details are missing and returns true if all course details are complete
bool CourseModule::dataComplete() const {
	if (courseTitle == "") return false;
	if (courseNumber == NULL) return false;
	if (units == NULL) return false;
	if (courseTitle == "") return false;
	if (courseDescription == "") return false;
	return true;
}

bool CourseModule::operator==(const CourseModule &c) const {
	return (this->courseSubject == c.courseSubject) && (this->courseNumber == c.courseNumber);
}

bool CourseModule::operator<(const CourseModule &c) const {
	if (this->courseSubject < c.courseSubject) return true;
	if ((this->courseSubject == c.courseSubject) && (this->courseNumber < c.courseNumber)) return true;
	return false;
}

bool CourseModule::operator>(const CourseModule &c) const {
	if (this->courseSubject > c.courseSubject) return true;
	if ((this->courseSubject == c.courseSubject) && (this->courseNumber > c.courseNumber)) return true;
	return false;
}

//Inserts course subject followed by course number into the ostream given
std::ostream &operator<<(std::ostream &os, const CourseModule &course) {
	os << course.courseSubject + " " + std::to_string(course.courseNumber);
	return os;
}
