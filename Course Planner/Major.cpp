#include <fstream>
#include "Major.h"
#include "CourseData.h"

void Major::save() {
	saveMajorReqs();
	saveElectives();
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


void Major::loadElectives() {
	//Load the graph containing the choice courses
	CourseData::load(electivesGraph, major + "_electives_data.txt");

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

Major::Major(std::string majorAcronym, std::string title) {
	//Trimming leading and trailing spaces of title
	title = title.substr(title.find_first_not_of(" "));
	title = title.substr(0, title.find_last_not_of(" ") + 1);
	majorTitle = title;

	//Trimming leading and trailing spaces of majorAcronym
	majorAcronym = majorAcronym.substr(majorAcronym.find_first_not_of(" "));
	majorAcronym = majorAcronym.substr(0, majorAcronym.find_last_not_of(" ") + 1);

	//Capitalizing majorAcronym
	for (auto &i : majorAcronym)
		i = std::toupper(i);

	major = majorAcronym;

	std::ifstream fileExistsChecker;
	std::ofstream outFile;

	fileExistsChecker.open(dataPath + major + "_major_requirements.txt");
	if (!fileExistsChecker.is_open()) {
		outFile.open(dataPath + major + "_major_requirements.txt");
		outFile.close();
	}
	fileExistsChecker.close();

	fileExistsChecker.open(dataPath + major + "_electives_data.txt");
	if (!fileExistsChecker.is_open()) {
		outFile.open(dataPath + major + "_electives_data.txt");
		outFile.close();
	}
	fileExistsChecker.close();

	fileExistsChecker.open(dataPath + major + "_elective_groups.txt");
	if (!fileExistsChecker.is_open()) {
		outFile.open(dataPath + major + "_elective_groups.txt");
		outFile.close();
	}
	fileExistsChecker.close();

	loadMajor();
}

void Major::loadMajor() {
	CourseData::load(majorRequirements, major + "_major_requirements.txt");
	loadElectives();
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

CourseGraph Major::getMajorReq() const {
	return majorRequirements;
}

std::list<ElectiveGroup> Major::getElectives() const {
	return electiveGroups;
}
