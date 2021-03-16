#include <vector>
#include <fstream>
#include <list>
#include "SchedulePreferenceData.h"
#include "CourseModule.h"

void SchedulePreferenceData::load(Scheduler &scheduler, const CourseGraph &g, std::string fileName) throw(std::runtime_error) {
	
	std::ifstream inFile;
	inFile.open(fileName);
	
	if (!inFile.is_open()) throw std::runtime_error("File \"" + fileName + "\" not found.");

	std::string currentLine;
	std::string data;


	//Read completed courses
	std::getline(inFile, currentLine);
	data = currentLine.substr(currentLine.find_first_of(":") + 1, std::string::npos);
	std::list<CourseModule> courses;
	while (data.size() > 1) {
		data = data.substr(1, std::string::npos);
		courses.push_back(CourseModule(data.substr(0, data.find_first_of(","))));
		data = data.substr(data.find_first_of(","), std::string::npos);
	}

	scheduler = Scheduler(g, courses);

	//Read overall unit limit
	std::getline(inFile, currentLine);
	scheduler.setUnitLimit(std::stoi(currentLine.substr(currentLine.find_last_of(" ") + 1, std::string::npos)));

	//Read season restrictions
	std::getline(inFile, currentLine);
	scheduler.setWinterAllowed(currentLine[currentLine.size() - 1] == 'T');
	std::getline(inFile, currentLine);
	scheduler.setSpringAllowed(currentLine[currentLine.size() - 1] == 'T');
	std::getline(inFile, currentLine);
	scheduler.setSummerAllowed(currentLine[currentLine.size() - 1] == 'T');
	std::getline(inFile, currentLine);
	scheduler.setFallAllowed(currentLine[currentLine.size() - 1] == 'T');
	


	//Read restricted semesters
	std::getline(inFile, currentLine); //Gets rid of "Restricted Semesters:"
	while (!inFile.eof()) {
		std::getline(inFile, currentLine);

		if (currentLine == "-" || currentLine == "") continue;

		Semester sem;
		
		std::string season = currentLine.substr(0, currentLine.find_first_of(" "));
		for (auto &i : season) i = std::tolower(i);
		if (season == "winter") sem.season = Semester::WINTER;
		else if (season == "spring") sem.season = Semester::SPRING;
		else if (season == "summer") sem.season = Semester::SUMMER;
		else sem.season = Semester::FALL;
		

		data = currentLine.substr(currentLine.find_first_of(" ") + 1, std::string::npos);
		sem.year = std::stoi(data.substr(0, data.find_first_of(":")));

		//Set semester max units
		std::getline(inFile, currentLine);
		scheduler.setSemesterUnitLimit(sem, std::stoi(currentLine.substr(currentLine.find_first_of("0123456789"), std::string::npos)));

		//Read restricted courses
		std::getline(inFile, currentLine);
		data = currentLine.substr(currentLine.find_first_of(":") + 2, std::string::npos);
		std::list<CourseModule> resCourses;
		while (data.size() > 1) {
			data = data.substr(1, std::string::npos);
			resCourses.push_back(CourseModule(data.substr(0, data.find_first_of(","))));
			data = data.substr(data.find_first_of(","), std::string::npos);
		}
		for (const auto &i : resCourses) scheduler.addRestriction(i, sem.season, sem.year);

		std::getline(inFile, currentLine);
		sem.semesterRestricted = (currentLine[currentLine.size() - 1] == 'T');
		scheduler.addRestriction(sem);
	}
	inFile.close();
}

void SchedulePreferenceData::store(const Scheduler &scheduler, std::string fileName) {
	std::ofstream outFile;
	outFile.open(fileName);

	outFile << "Completed Courses: ";

	for (const auto &i : scheduler.getCompletedCourses()) {
		outFile << i << ",";
	}
	outFile << std::endl;

	outFile << "unitLimit = " << scheduler.getMaxUnits() << std::endl;

	outFile << "winter = " << ((scheduler.getWinterAllowed()) ? "T" : "F") << std::endl;
	outFile << "spring = " << ((scheduler.getSpringAllowed()) ? "T" : "F") << std::endl;
	outFile << "summer = " << ((scheduler.getSummerAllowed()) ? "T" : "F") << std::endl;
	outFile << "fall = " << ((scheduler.getFallAllowed()) ? "T" : "F") << std::endl;

	outFile << "Restricted Semesters:" << std::endl;
	for (Semester i : scheduler.getRestrictedSemesters()) {
		outFile << i.ToString() << ":" << std::endl;
		outFile << "maxUnits = " << i.maxUnits << std::endl;

		outFile << std::endl << "restricted: ";
		for (const auto &c : i.restricted) {
			outFile << c << ",";
		}

		outFile << std::endl << "full restriction: " << ((i.semesterRestricted) ? "T" : "F") << std::endl;

		outFile << "-" << std::endl;
	}
	outFile.close();
}
