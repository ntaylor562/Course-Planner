#ifndef COURSE_MODULE_H
#define COURSE_MODULE_H
#include <vector>
#include <string>
#include <iostream>


/*
* Represents a course which has a course subject, course number, course title, number of units, description, and can have prerequisites
* Keeps track of all the courses that this course is a prerequisite for
*/
class CourseModule {
private:
	//The set of 
	std::vector<CourseModule*> prerequisites;
	std::vector<CourseModule*> prerequisiteFor;
	int units, courseNumber;
	std::string courseSubject, courseTitle = "", courseDescription = "";

	void addPrerequisiteFor(CourseModule* course);
	void removePrerequisiteFor(CourseModule* course);
public:
	CourseModule(std::string subject = "", int number = 0, std::string title = "", int numOfUnits = 0, std::string description = "", std::vector<CourseModule *> prereqs = std::vector<CourseModule*>());
	~CourseModule();

	void setCourseSubject(std::string str);
	void setCourseNumber(int num);
	void setCourseTitle(std::string str);
	void setUnits(int num);
	void setDescription(std::string);

	std::string getCourseSubject();
	int getCourseNumber();
	std::string getCourseTitle();
	int getUnits();
	std::string getDescription();
	std::string getPrereqsAsString(); //Returns "" if empty
	std::string getPrereqForAsString(); //Returns "" if empty
	std::string getCourseName();

	void addPrerequisite(CourseModule* course);
	void removePrerequisite(CourseModule* course);
	std::vector<CourseModule*>* getPrerequisites();
	std::vector<CourseModule*>* getPrerequisiteFor();

	bool dataComplete();

	friend std::ostream &operator<<(std::ostream &os, const CourseModule &course);
};

#endif
