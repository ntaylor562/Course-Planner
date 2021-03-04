#include <string>
#include <vector>
#include <iostream>//DEBUGGING
#include "CourseModule.h"

//Constructor
CourseModule::CourseModule(std::string subject, int number, std::string title, int numOfUnits, std::string description, std::vector<CourseModule*> prereqs) :
	courseSubject(subject), courseNumber(number), courseTitle(title), units(numOfUnits), courseDescription(description), prerequisites(prereqs) {}

//Destructor removing this course from other course's prerequisites
CourseModule::~CourseModule() {
	if (!prerequisiteFor.empty()) {

		for (std::vector<CourseModule*>::iterator it = prerequisiteFor.begin(); it != prerequisiteFor.end(); ++it) { //Looping through all the courses that this course is a prerequisite for
			
			for (std::vector<CourseModule*>::iterator i = (*it)->getPrerequisites()->begin(); i != (*it)->getPrerequisites()->end(); ++i) {//Finding this course in the prerequisiteFor's list of prerequisites and removing it from its prerequisites
				if (*i == this) {
					(*it)->getPrerequisites()->erase(i);
					break;
				}
			} //I did this instead of calling the course's removePrerequisite because that function also removes itself from THIS course's prerequisiteFor
		
		}

	}
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

//Returns a string containing the prerequisites' course subjects and numbers
std::string CourseModule::getPrereqsAsString() const {
	std::string prereqs;
	if (!prerequisites.empty()) {
		for (std::vector<CourseModule *>::const_iterator it = prerequisites.begin(); it != prerequisites.end() - 1; ++it) {
			prereqs += (*it)->getCourseSubject() + " " + std::to_string((*it)->getCourseNumber()) + " ";
		}
		prereqs += (*(prerequisites.end() - 1))->getCourseSubject() + " " + std::to_string((*(prerequisites.end() - 1))->getCourseNumber());
	}
	return prereqs;
}

//Adds course to list of prerequisites using a linear search to determine repeats (will improve in future)
void CourseModule::addPrerequisite(CourseModule* course) {
	if (course == this) return;
	for (std::vector<CourseModule*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++it) {
		if (course == *it) return;
	} //Ensures no duplicates
	prerequisites.push_back(course);
	course->addPrerequisiteFor(this);
}

//Removes course from this course's list of prerequisites and does nothing if course is not a prerequisite
void CourseModule::removePrerequisite(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++it) {
		if (*it == course) {
			(*it)->removePrerequisiteFor(this);
			prerequisites.erase(it);
			return;
		}
	}
}

//Returns pointer to vector containing this course's prerequisites
std::vector<CourseModule*>* CourseModule::getPrerequisites() {
	return &prerequisites;
}

//Adds course to the list of courses this course is a prerequisite for and uses linear search to determine repeats (will improve in future)
void CourseModule::addPrerequisiteFor(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisiteFor.begin(); it != prerequisiteFor.end(); ++it) {
		if (course == *it) return;
	} //Ensures no duplicates
	prerequisiteFor.push_back(course);
}

//Removes this course from course's prerequisites
void CourseModule::removePrerequisiteFor(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisiteFor.begin(); it != prerequisiteFor.end(); ++it) {
		if (*it == course) {
			prerequisiteFor.erase(it);
			return;
		}
	}
}

//Returns pointer to vector containing courses this course is a prerequisite for
std::vector<CourseModule*>* CourseModule::getPrerequisiteFor() {
	return &prerequisiteFor;
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
