#include <vector>
#include <fstream>
#include "CourseData.h"

void CourseData::load(CourseGraph &graph, std::string fileName) throw(std::runtime_error) {
	if (fileName == ".txt") throw std::runtime_error("Invalid file name.");
	else if (fileName.find(".txt") == std::string::npos) fileName += ".txt";

	std::ifstream inFile;
	inFile.open(fileName);

	if (!inFile.is_open()) throw std::runtime_error("File \"" + fileName + "\" not found.");

	std::string currentLine;
	std::string whatIsIt; //Example being "Course Subject" or "Units". This is so we can read the data correctly
	std::string data; //This is the actual data in the line that we'll be reading

	//if (inFile.peek() == EOF) throw std::runtime_error("File \"" + fileName + "\" is empty.");

	try {
		while (!inFile.eof()) {
			if (inFile.peek() == EOF) {
				inFile.close();
				return;
			}

			std::getline(inFile, currentLine);
			std::string cdata = currentLine.substr(currentLine.find(':') + 2, std::string::npos); //Course subject and number

			vertex *v = *graph.insert(CourseModule(cdata)); //Vertex we're adding data to
			std::getline(inFile, currentLine);
			if (currentLine.length() > 1) {
				whatIsIt = currentLine.substr(0, currentLine.find(':'));
				data = currentLine.substr(currentLine.find(':') + 2, std::string::npos);
				data = data.substr(0, data.find_last_not_of(" ") + 1);
			}

			while (currentLine.length() > 1) { //Initialize the current course
				if (whatIsIt == "Course Title") {
					v->course.setCourseTitle(data);
				}
				else if (whatIsIt == "Units") {
					v->course.setUnits(std::stoi(data));
				}
				else if (whatIsIt == "Course Description") {
					v->course.setDescription(data);
				}
				else if (whatIsIt == "Prerequisites") {

					if (data != "") { //Read prerequisites
						std::string sub;
						int num = 0;

						do {
							sub = data.substr(0, data.find_first_of("1234567890") - 1);
							data = data.substr(data.find_first_of("1234567890"), std::string::npos);
							std::size_t endNum = data.find_first_not_of("1234567890");

							if (endNum != std::string::npos) {
								num = std::stoi(data.substr(0, endNum));
								data = data.substr(endNum + 1, data.size() - 1);
							}
							else {
								num = std::stoi(data);
								data = "";
							}

							graph.addEdge(CourseModule(sub, num), v->course);

						} while (data.find(' ') != std::string::npos);
					}
				}

				std::getline(inFile, currentLine);
				if (currentLine.length() > 1) {
					whatIsIt = currentLine.substr(0, currentLine.find(':'));
					data = currentLine.substr(currentLine.find(':') + 2, currentLine.length() - 1);
				}
			}
		}
	}
	catch (...) {
		throw std::runtime_error("Error reading data file.");
	}
	inFile.close();
}

void CourseData::store(const CourseGraph &graph, std::string fileName) {
	std::ofstream outFile;

	outFile.open(fileName);

	for (std::vector<vertex *>::const_iterator it = graph.begin(); it != graph.end(); ++it) {
		
		outFile << "Course: " << (*it)->course << std::endl;
		
		if ((*it)->course.getCourseTitle() != "") //Save course title
			outFile << "Course Title: " << (*it)->course.getCourseTitle() << std::endl;
		
		if ((*it)->course.getUnits() != NULL) //Save course units
			outFile << "Units: " << (*it)->course.getUnits() << std::endl;
		
		if ((*it)->course.getDescription() != "") //Save course description
			outFile << "Course Description: " << (*it)->course.getDescription() << std::endl;
		
		if (!(*it)->prerequisites.empty()) { //Save prerequisites
			outFile << "Prerequisites: ";

			for (vertex *i : (*it)->prerequisites) {
				outFile << i->course << " ";
			}
			outFile << std::endl;
		}

		outFile << "-" << std::endl;
	}

	outFile.close();
}
