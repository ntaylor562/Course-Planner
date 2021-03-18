#ifndef MAJOR_H
#define MAJOR_H

#include <string>
#include <list>
#include "CourseGraph.h"

class Major {
private:

	std::string major;
	std::string majorTitle;
	CourseGraph majorRequirements;
	CourseGraph electives;

	//List of lists containing graphs where there is an option (Ex: CECS majors can choose PHYS 151 or CHEM 1111)
	//Each graph should have only 1 vertex with an outdegree of 0
	//The choice courses are represented as graphs so we could track the prerequisites it may have
	std::list<std::list<CourseGraph>> choiceCourses;

	//Saves all data into their respective files
	void save();

	void saveMajorReqs();
	void saveElectives();
	void saveChoiceCourses();

	//Used to get a vertex in graph g with an outdegree of 0. There should only be one of these in a choice course graph
	vertex *getLeaf(const CourseGraph &g);


public:

	/**
	 * @brief Constructor using the name of the major (Ex: CECS) to name the major, elective, and choice course files
	 * @details Reads majorName_major_requirements.txt for major requirements
	 * @details Reads majorName_electives.txt for elective courses
	 * @details Reads majorName_choice_courses.txt for courses the student has a choice in (not electives)
	 * @param majorName Name of the major (Ex: CECS, MATH, ENGR, etc.)
	*/
	Major(std::string majorAcronym, std::string title);

	std::string getMajor() const;
	std::string getTitle() const;

	CourseGraph getMajorReq() const;
	CourseGraph getElectives() const;
	std::list<std::list<CourseGraph>> getChoiceCourses();

	//Adds a course to the major requirements graph
	void addMajorReq(const CourseModule &c);

	//Adds a vertex to the major requirements graph
	void addMajorReq(const vertex &v);

	//Merges the major requirements graph with g
	void addMajorReq(const CourseGraph &g);

	//Adds a course to the elective graph
	void addElective(const CourseModule &c);
	
	//Adds a vertex to the elective graph
	void addElective(const vertex &v);

	//Merges the elective graph with g
	void addElective(const CourseGraph &g);

	/**
	 * @brief Adds the list to the list of choice lists
	 * @param choices List of courses that the student has an option to take (Ex: CECS majors have to choose between PHYS 151 or CHEM 1111)
	*/
	void addChoice(const std::list<CourseGraph> &choices);
};

#endif