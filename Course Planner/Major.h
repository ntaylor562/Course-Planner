#ifndef MAJOR_H
#define MAJOR_H

#include <string>
#include <list>
#include "CourseGraph.h"

#define majorDataPath "majors/"

struct ElectiveGroup {

	//List of electives the user has to choose from in a group of courses
	std::list<vertex *> electives;

	//Minimum units the student must fill in this group
	int unitMinimum;

};

class Major {
private:

	//Path of the major files
	std::string path;

	std::string major;
	std::string majorTitle;
	CourseGraph majorRequirements;

	//Represents a list of elective groups user has to pick from
	std::list<ElectiveGroup> electiveGroups;

	//Graph containing all the vertices of the elective courses
	CourseGraph electivesGraph;

	//Saves all data into their respective files
	void save();

	void saveMajorReqs();
	void saveElectives();

	void loadElectives();


public:

	/**
	 * @brief Constructor using the name of the major (Ex: CECS) to name the major, elective, and choice course files
	 * @details Reads majorName_major_requirements.txt for major requirements
	 * @details Reads majorName_electives.txt for elective courses
	 * @details Reads majorName_choice_courses.txt for courses the student has a choice in (not electives)
	 * @param majorName Name of the major (Ex: CECS, MATH, ENGR, etc.)
	*/
	Major(std::string majorAcronym, std::string title) throw (std::runtime_error);

	/**
	 * @brief Loading the values for this major from the appropriate files
	 * @param majorAcronym The acronym representing the major (Ex: CECS)
	*/
	void loadMajor();

	std::string getPath() const;
	std::string getMajor() const;
	std::string getTitle() const;

	void addMajorReq(const CourseModule &c);
	void addMajorReq(const vertex &v);
	void addMajorReq(const CourseGraph &g);

	void addElectiveGroup(const ElectiveGroup &e);

	CourseGraph getMajorReq() const;
	std::list<ElectiveGroup> getElectives() const;
};

#endif