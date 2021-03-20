#include <fstream>
#include "Major.h"
#include "CourseData.h"

void Major::save() {
	saveMajorReqs();
	saveElectives();
	saveChoiceCourses();
}

void Major::saveMajorReqs() {
	CourseData::store(majorRequirements, major + "_major_requirements.txt");
}

void Major::saveElectives() {
	CourseData::store(electivesGraph, major + "_electives_data.txt");

	std::ofstream outFile;
	outFile.open(major + "_elective_groups.txt");

	for (const auto &e : electiveGroups) {
		outFile << "Unit minimum = " << e.unitMinimum << std::endl << ";";
		for (const auto &v : e.electives) {
			outFile << v->course << ";";
		}

		outFile << std::endl;
	}
}

void Major::saveChoiceCourses() {
	CourseData::store(choiceCoursesGraph, major + "_choice_course_data.txt"); //Save choice course graph

	std::ofstream outFile;
	outFile.open(major + "_choice_course_list.txt");

	for (const auto &lst : choiceCourses) {
		outFile << ";";
		for (const auto &v : lst) {
			outFile << v->course << ";";
		}
		outFile << std::endl;
	}
}

void Major::loadElectives() {
	//Load the graph containing the choice courses
	CourseData::load(choiceCoursesGraph, major + "_electives_data.txt");

	std::ifstream inFile;
	inFile.open(dataPath + major + "_elective_groups.txt");
	if (!inFile.is_open()) throw std::runtime_error("\"" + major + "_elective_groups.txt\" file not found.");

	std::string currentLine; //Line of the text file we're currently reading
	std::getline(inFile, currentLine);
	while (!inFile.eof()) {

		if (currentLine == "") return; //In case the loop gets stuck on a blank line at the end of the file

		int unitMin = std::stoi(currentLine.substr(currentLine.find_first_of("0123456789")));

		std::getline(inFile, currentLine);

		std::list<vertex *> currentList; //The vertices the user must choose from
		while (currentLine.size() > 1) {
			//Name of the course we're reading
			currentLine = currentLine.substr(1);
			std::string courseName = currentLine.substr(0, currentLine.find_first_of(";"));
			//Add the course to the current list of choice courses
			currentList.push_back(electivesGraph.search(CourseModule(courseName)));
			//Iterate
			currentLine = currentLine.substr(currentLine.find_first_of(";"));
		}
		electiveGroups.push_back(ElectiveGroup{ currentList, unitMin});
		//Read next list of choice courses
		std::getline(inFile, currentLine);
	}
	inFile.close();
}

void Major::loadChoiceCourses() {
	//Load the graph containing the choice courses
	CourseData::load(choiceCoursesGraph, major + "_choice_courses_data.txt");

	std::ifstream inFile;
	inFile.open(dataPath + major + "_choice_course_list.txt");
	if (!inFile.is_open()) throw std::runtime_error("\"" + major + "_choice_course_list.txt\" file not found.");

	std::string currentLine; //Line of the text file we're currently reading
	std::getline(inFile, currentLine);
	while (!inFile.eof()) {

		if (currentLine == "") return; //In case the loop gets stuck on a blank line at the end of the file

		std::list<vertex *> currentList; //The vertices the user must choose from
		while (currentLine.size() > 1) {
			//Name of the course we're reading
			currentLine = currentLine.substr(1);
			std::string courseName = currentLine.substr(0, currentLine.find_first_of(";"));
			//Add the course to the current list of choice courses
			currentList.push_back(choiceCoursesGraph.search(CourseModule(courseName)));
			//Iterate
			currentLine = currentLine.substr(currentLine.find_first_of(";"));
		}
		choiceCourses.push_back(currentList);
		//Read next list of choice courses
		std::getline(inFile, currentLine);
	}
	inFile.close();
}

Major::Major(std::string majorAcronym, std::string title) {
	majorTitle = title;
	for (auto &i : majorAcronym)
		i = std::toupper(i);
	major = majorAcronym;

	CourseData::load(majorRequirements, major + "_major_requirements.txt");
	loadElectives();
	loadChoiceCourses();
}

std::string Major::getMajor() const {
	return major;
}

std::string Major::getTitle() const {
	return majorTitle;
}

void Major::addMajorReq(const CourseModule &c) {
	majorRequirements.insert(c);
	saveMajorReqs();
}

void Major::addMajorReq(const vertex &v) {
	majorRequirements.insert(v);
	saveMajorReqs();
}

void Major::addMajorReq(const CourseGraph &g) {
	majorRequirements.merge(g);
	saveMajorReqs();
}

void Major::addElectiveGroup(const ElectiveGroup &e) {
	ElectiveGroup newGroup;
	for (const auto &v : e.electives) {
		newGroup.electives.push_back(*electivesGraph.insert(v->course));
	}

	electiveGroups.push_back(newGroup);
	saveElectives();
}

void Major::addCourseChoice(const std::list<vertex *> &choices) {
	std::list<vertex *> newChoices;
	for (const auto &v : choices) {
		newChoices.push_back(*choiceCoursesGraph.insert(*v));
	}

	choiceCourses.push_back(newChoices);
	saveChoiceCourses();
}

CourseGraph Major::getMajorReq() const {
	return majorRequirements;
}

std::list<ElectiveGroup> Major::getElectives() const {
	return electiveGroups;
}

std::list<std::list<vertex *>> Major::getChoiceCourses() const {
	return choiceCourses;
}
