#include <string>
#include <vector>
#include <iostream>//DEBUGGING
#include "CourseModule.h"


CourseModule::CourseModule(std::string subject, int number, std::string title, int numOfUnits, std::string description, std::vector<CourseModule*> prereqs) :
	courseSubject(subject), courseNumber(number), courseTitle(title), units(numOfUnits), courseDescription(description), prerequisites(prereqs) {}

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

void CourseModule::setCourseSubject(std::string str) { courseSubject = str; }

void CourseModule::setCourseNumber(int num) { courseNumber = (num >= 0) ? num : 0; }

void CourseModule::setCourseTitle(std::string str) { courseTitle = str; }

void CourseModule::setUnits(int num) { units = (num >= 0) ? num : 0; }

void CourseModule::setDescription(std::string str) { courseDescription = str; }

void CourseModule::addPrerequisite(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++it) {
		if (course == *it) return;
	} //Ensures no duplicates
	prerequisites.push_back(course);
	course->addPrerequisiteFor(this);
}

void CourseModule::addPrerequisiteFor(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisiteFor.begin(); it != prerequisiteFor.end(); ++it) {
		if (course == *it) return;
	} //Ensures no duplicates
	prerequisiteFor.push_back(course);
}

void CourseModule::removePrerequisite(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisites.begin(); it != prerequisites.end(); ++it) {
		if (*it == course) {
			(*it)->removePrerequisiteFor(this);
			prerequisites.erase(it);
			return;
		}
	}
}

void CourseModule::removePrerequisiteFor(CourseModule* course) {
	for (std::vector<CourseModule*>::iterator it = prerequisiteFor.begin(); it != prerequisiteFor.end(); ++it) {
		if (*it == course) {
			prerequisiteFor.erase(it);
			return;
		}
	}
}

std::vector<CourseModule*>* CourseModule::getPrerequisites() {
	return &prerequisites;
}

std::vector<CourseModule*>* CourseModule::getPrerequisiteFor() {
	return &prerequisiteFor;
}

std::string CourseModule::getCourseName() { 
	return courseSubject + " " + std::to_string(courseNumber) + " - " + courseTitle;
}

std::string CourseModule::getCourseSubject() { return courseSubject; }

int CourseModule::getCourseNumber() { return courseNumber; }

std::string CourseModule::getCourseTitle() { return courseTitle; }

int CourseModule::getUnits() { return units; }

std::string CourseModule::getDescription() { return courseDescription; }

std::string CourseModule::getPrereqsAsString() {
	std::string prereqs;
	if (!prerequisites.empty()) {
		for (std::vector<CourseModule*>::iterator it = prerequisites.begin(); it != prerequisites.end() - 1; ++it) {
			prereqs += (*it)->getCourseSubject() + " " + std::to_string((*it)->getCourseNumber()) + " ";
		}
		prereqs += (*(prerequisites.end() - 1))->getCourseSubject() + " " + std::to_string((*(prerequisites.end() - 1))->getCourseNumber());
	}
	return prereqs;
}

std::string CourseModule::getPrereqForAsString() {
	std::string prereqs;
	if (!prerequisiteFor.empty()) {
		for (std::vector<CourseModule*>::iterator it = prerequisiteFor.begin(); it != prerequisiteFor.end() - 1; ++it) {
			prereqs += (*it)->getCourseSubject() + " " + std::to_string((*it)->getCourseNumber()) + " ";
		}
		prereqs += (*(prerequisiteFor.end() - 1))->getCourseSubject() + " " + std::to_string((*(prerequisiteFor.end() - 1))->getCourseNumber());
	}
	return prereqs;
}

bool CourseModule::dataComplete() {
	if (courseTitle == "") return false;
	if (units == NULL) return false;
	if (courseDescription == "") return false;
	return true;
}

std::ostream &operator<<(std::ostream &os, const CourseModule &course) {
	os << course.courseSubject << " " << course.courseNumber;
	return os;
}
