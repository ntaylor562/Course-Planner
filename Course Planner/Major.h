#ifndef MAJOR_H
#define MAJOR_H

#include <string>
#include <list>
#include "CourseGraph.h"


struct ElectiveGroup {
	//List of electives the user has to choose from in a group of courses
	std::list<vertex *> electives;

	//Minimum units the student must fill in this group
	int unitMinimum;

};

class Major {
private:

	std::string major;
	std::string majorTitle;
	CourseGraph majorRequirements;

	//Represents a list of elective groups user has to pick from
	std::list<ElectiveGroup> electiveGroups;

	//Graph containing all the vertices of the elective courses
	CourseGraph electivesGraph;

	//List of lists containing courses where there is an option (Ex: CECS majors can choose PHYS 151 or CHEM 1111)
	//The choice courses are represented as pointers to vertices so we can easily insert them into another graph
	std::list<std::list<vertex *>> choiceCourses;

	//Graph containing all the vertices of the choice courses and their prerequisites
	CourseGraph choiceCoursesGraph;

	//Saves all data into their respective files
	void save();

	void saveMajorReqs();
	void saveElectives();
	void saveChoiceCourses();

	void loadElectives();
	void loadChoiceCourses();


public:

	/**
	 * @brief Constructor using the name of the major (Ex: CECS) to name the major, elective, and choice course files
	 * @details Reads majorName_major_requirements.txt for major requirements
	 * @details Reads majorName_electives.txt for elective courses
	 * @details Reads majorName_choice_courses.txt for courses the student has a choice in (not electives)
	 * @param majorName Name of the major (Ex: CECS, MATH, ENGR, etc.)
	*/
	Major(std::string majorAcronym, std::string title) throw (std::runtime_error);

	std::string getMajor() const;
	std::string getTitle() const;

	void addMajorReq(const CourseModule &c);
	void addMajorReq(const vertex &v);
	void addMajorReq(const CourseGraph &g);

	void addElectiveGroup(const ElectiveGroup &e);

	void addCourseChoice(const std::list<vertex *> &choices);

	CourseGraph getMajorReq() const;
	std::list<ElectiveGroup> getElectives() const;
	std::list<std::list<vertex *>> getChoiceCourses() const;
};

#endif