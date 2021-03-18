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
	CourseData::store(electives, major + "_electives.txt");
}

void Major::saveChoiceCourses() {
	std::ofstream outFile;
	outFile.open(major + "_choice_course_index.txt");
	for (const auto &lst : choiceCourses) {
		outFile << ";";
		for (const auto &g : lst) {
			vertex *v = getLeaf(g);
			std::string fileName = v->course.getCourseSubject() + "_" + std::to_string(v->course.getCourseNumber()) + "_choice_course_data.txt";
			CourseData::store(g, fileName);
			outFile << fileName << ";";
		}
		outFile << std::endl;
	}
}

vertex *Major::getLeaf(const CourseGraph &g) {
	vertex *v = *g.begin();
	while (!v->prerequisiteFor.empty())
		v = v->prerequisiteFor.front();
	return v;
}

Major::Major(std::string majorAcronym, std::string title) {
	majorTitle = title;
	for (auto &i : majorAcronym)
		i = std::toupper(i);
	major = majorAcronym;

	CourseData::load(majorRequirements, majorAcronym + "_major_requirements.txt");
	CourseData::load(electives, majorAcronym + "_electives.txt");
	
	std::ifstream indexFile;
	indexFile.open(dataPath + major + "_choice_course_index.txt");
	if (!indexFile.is_open()) throw std::runtime_error("\"" + major + "_choice_course_index.txt\" file not found.");

	std::string currentLine; //Line of the text file we're currently reading
	std::getline(indexFile, currentLine);
	while (!indexFile.eof()) {
		if (currentLine == "") return; //In case the loop gets stuck on a blank line at the end of the file
		std::list<CourseGraph> currentList; //The graphs the user much choose from
		while (currentLine.size() > 1) {
			CourseGraph g;
			CourseData::load(g, currentLine.substr(1, currentLine.find_first_of(";"))); //Load every graph with the associated file
			currentList.push_back(g);
			currentLine = currentLine.substr(currentLine.find_first_of(";"));
		}
		std::getline(indexFile, currentLine);
		choiceCourses.push_back(currentList);
	}
	indexFile.close();
}

std::string Major::getMajor() const {
	return major;
}

std::string Major::getTitle() const {
	return majorTitle;
}

CourseGraph Major::getMajorReq() const {
	return majorRequirements;
}

CourseGraph Major::getElectives() const {
	return electives;
}

std::list<std::list<CourseGraph>> Major::getChoiceCourses() {
	return choiceCourses;
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

void Major::addElective(const CourseModule &c) {
	electives.insert(c);
	saveElectives();
}

void Major::addElective(const vertex &v) {
	electives.insert(v);
	saveElectives();
}

void Major::addElective(const CourseGraph &g) {
	electives.merge(g);
	saveElectives();
}

void Major::addChoice(const std::list<CourseGraph> &choices) {
	choiceCourses.push_back(choices);
	saveChoiceCourses();
}
