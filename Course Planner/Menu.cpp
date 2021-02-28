#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "Menu.h"
#include "CourseModule.h"
#include "CourseInputChecker.h"

Menu::Menu() {
	readData();
}

void Menu::readData() {
	inFile.open(dataFile);

	if (!inFile.is_open()) {
		std::cout << "The file " << dataFile << " does not exist. Would you like to create one? (y/n)" << std::endl;

		bool createNewFile = InputChecker::getBool("Input invalid. Create file? (y/n) ");
		std::cout << std::endl;

		if (createNewFile) {
			std::ofstream file;
			file.open(dataFile);
			file.close();
			std::cout << dataFile << " has been created." << std::endl;
			system("pause");
			system("cls");
		}
		else {
			std::cout << "No file to read. Exiting" << std::endl;
			system("pause");
			inFile.close();
			std::exit(0);
		}
	}

	std::string subject;
	int number;

	std::string currentLine;
	std::string whatIsIt; //Example being "Course Subject" or "Units". This is so that I can read the data correctly
	std::string data; //This is the actual data in the line that I'll be reading

	CourseModule* course;
	while (!inFile.eof()) {
		if (inFile.peek() == EOF) {
			inFile.close();
			return;
		}

		std::getline(inFile, currentLine);
		subject = currentLine.substr(currentLine.find(':') + 2, currentLine.length() - 1);
		
		std::getline(inFile, currentLine);
		number = std::stoi(currentLine.substr(currentLine.find(':') + 2, currentLine.length() - 1));

		course = addCourse(subject, number);

		std::getline(inFile, currentLine);
		if (currentLine.length() > 1) {
			whatIsIt = currentLine.substr(0, currentLine.find(':'));
			data = currentLine.substr(currentLine.find(':') + 2, currentLine.length() - 1);
		}

		while (currentLine.length() > 1) {
			if (whatIsIt == "Course Title") {
				course->setCourseTitle(data);
			}
			else if (whatIsIt == "Units") {
				course->setUnits(std::stoi(data));
			}
			else if (whatIsIt == "Course Description") {
				course->setDescription(data);
			}
			else if (whatIsIt == "Prerequisites") {
				
				if (data != "") {
					readPrerequisites(course, data);
				}
				
			}

			std::getline(inFile, currentLine);
			if (currentLine.length() > 1) {
				whatIsIt = currentLine.substr(0, currentLine.find(':'));
				data = currentLine.substr(currentLine.find(':') + 2, currentLine.length() - 1);
			}
		}
	}
	inFile.close();
}

void Menu::readPrerequisites(CourseModule* course, std::string str) {
	std::string sub;
	int num = 0;

	do {
		sub = str.substr(0, str.find_first_of("1234567890") - 1);
		str = str.substr(str.find_first_of("1234567890"), str.length() - 1);
		std::size_t endNum = str.find_first_not_of("1234567890");

		if (endNum != std::string::npos) {
			num = std::stoi(str.substr(0, endNum));
			str = str.substr(endNum + 1, str.size() - 1);
		}
		else {
			num = std::stoi(str);
			str = "";
		}

		CourseModule *temp = addCourse(sub, num);
		course->addPrerequisite(temp);
	} while (str.find(' ') != std::string::npos);
}

void Menu::saveData() {
	outFile.open(dataFile);
	for (std::vector<CourseModule*>::iterator it = Courses.begin(); it != Courses.end(); ++it) {
		outFile << "Course Subject: " << (*it)->getCourseSubject() << std::endl
			<< "Course Number: " << (*it)->getCourseNumber() << std::endl;
		if ((*it)->getCourseTitle() != "")
			outFile << "Course Title: " << (*it)->getCourseTitle() << std::endl;
		if ((*it)->getUnits() != NULL)
			outFile << "Units: " << (*it)->getUnits() << std::endl;
		if ((*it)->getDescription() != "")
			outFile << "Course Description: " << (*it)->getDescription() << std::endl;
		if (!(*it)->getPrerequisites()->empty())
			outFile << "Prerequisites: " << (*it)->getPrereqsAsString() << std::endl;
		outFile << "-" << std::endl;
	}
	outFile.close();
}

void Menu::runMenu() {
	std::cout << "Welcome to Course Manager" << std::endl
		<< "This application allows you to add a list of courses and their information\nto help manage all the classes you "
		<< "need to take." << std::endl << std::endl;

	mainMenu();
}

CourseModule* Menu::courseSearch(std::string sub, int num) {
	for (std::vector<CourseModule*>::iterator courseIterator = Courses.begin(); courseIterator != Courses.end(); ++courseIterator) {
		if ((*courseIterator)->getCourseSubject() == sub && (*courseIterator)->getCourseNumber() == num)
			return *courseIterator;
	}
	return nullptr;
}

CourseModule * Menu::courseSearch(std::string str) {
	return courseSearch(str.substr(0, str.find(' ')), std::stoi(str.substr(str.find(' ') + 1, str.length() - 1)));
}

std::vector<CourseModule*>::iterator Menu::courseSearchIt(std::string sub, int num) {
	std::vector<CourseModule*>::iterator courseIterator;
	for (courseIterator = Courses.begin(); courseIterator != Courses.end(); ++courseIterator) {
		if ((*courseIterator)->getCourseSubject() == sub && (*courseIterator)->getCourseNumber() == num)
			return courseIterator;
	}
	return courseIterator;
}

std::vector<CourseModule*>::iterator Menu::courseSearchIt(std::string str) {
	return courseSearchIt(str.substr(0, str.find(' ')), std::stoi(str.substr(str.find(' ') + 1, str.length() - 1)));
}

void Menu::printAllCourses() {
	int distance = 15;
	int coursesPerLine = 5;
	std::cout << std::left << "Current list of courses:\n\n";

	int count = 1;
	for (std::vector<CourseModule*>::iterator it = Courses.begin(); it != Courses.end(); ++it) {
		std::string course;
		if (!(*it)->dataComplete()) course += "~";
		course += (*it)->getCourseSubject() + " " + std::to_string((*it)->getCourseNumber());
		if (count <= coursesPerLine) {
			std::cout << std::setw(distance) << course;
			++count;
		} else {
			std::cout << std::endl << std::setw(distance) << course;
			count = 2;
		}
	}
	std::cout << std::endl;
}

void Menu::printAllIncompleteDataCourses() {
	std::vector<CourseModule*>::iterator it;
	bool empty = true;
	for (it = Courses.begin(); it != Courses.end(); ++it) {
		if (!(*it)->dataComplete()) {
			empty = false;
			break;
		}
	}
	if (empty) {
		std::cout << "Data for all courses is complete." << std::endl;
		return;
	}

	int distance = 15;
	int coursesPerLine = 5;
	std::cout << std::left << "Current list of courses with incomplete data:\n\n";

	int count = 1;
	for (std::vector<CourseModule*>::iterator it = Courses.begin(); it != Courses.end(); ++it) {
		std::string course;
		if (!(*it)->dataComplete()) {
			course += "~";
			course += (*it)->getCourseSubject() + " " + std::to_string((*it)->getCourseNumber());
			if (count <= coursesPerLine) {
				std::cout << std::setw(distance) << course;
				++count;
			} else {
				std::cout << std::endl << std::setw(distance) << course;
				count = 1;
			}
		}
	}
	std::cout << std::endl;
}

void Menu::printAllCourseData() {
	std::cout << "Current list of courses and their data:\n\n";
	std::vector<CourseModule*>::iterator it;
	for (it = Courses.begin(); it != Courses.end() - 1; ++it) {
		std::cout << fullCourseInfo(*it) << std::endl;
		std::cout << "-------------------------------------------------------------------------------" << std::endl << std::endl;
	}
	std::cout << fullCourseInfo(*(Courses.end() - 1)) << std::endl;
}

std::string Menu::fullCourseInfo(CourseModule* course) {
	std::string result;
	if (!course->dataComplete()) result += "~";
	result += course->getCourseSubject() + " " + std::to_string(course->getCourseNumber());

	if (course->getCourseTitle() != "") result += " - " + course->getCourseTitle();
	if (course->getUnits() > 0) result += "\nUnits: " + std::to_string(course->getUnits());
	if (course->getDescription() != "") result += "\nDescription: " + course->getDescription();
	if (course->getPrereqsAsString() != "") result += "\nPrerequisites: " + course->getPrereqsAsString();
	result += "\n";
	return result;
}

int Menu::readIntInput() {
	std::string input;


	std::getline(std::cin, input);
	if (input == "") return -1;

	for (int i = 0; i < input.length(); ++i) {
		if (input[i] < '0' || input[i] > '9') {
			return -1;
		}
	}
	return std::stoi(input);
}

void Menu::mainMenu() {
	bool exit = false;
	while (!exit) {
		std::cout << "Main Menu" << std::endl
			<< "0 - Exit" << std::endl
			<< "1 - Show courses" << std::endl
			<< "2 - Add a course" << std::endl
			<< "3 - Remove a course" << std::endl
			<< "4 - Edit an existing course" << std::endl
			<< "5 - Save courses" << std::endl << std::endl;

		std::cout << "Enter a choice: ";
		int userChoice = readIntInput();
		switch (userChoice) {
		case 0:
			exit = true;
			break;
		case 1:
			system("cls");
			printAllCourseData();
			std::cout << std::endl;
			printAllIncompleteDataCourses();
			std::cout << std::endl;

			system("pause");
			break;
		case 2:
			system("cls");
			subMenuCourseAdd();
			break;
		case 3:
			system("cls");
			subMenuCourseRemove();
			break;
		case 4:
			system("cls");
			subMenuCourseEdit();
			break;
		case 5:
			saveData();
			std::cout << "Data has been saved to " << dataFile << std::endl << std::endl;
			system("pause");
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			system("pause");
			break;
		}
		system("cls");
	}
}

void Menu::subMenuCourseAdd() {
	std::cout << "Add a Course" << std::endl << std::endl;
	printAllCourses();
	std::cout << std::endl;
	CourseModule *course = inputNewValidCourse();
	if (course == nullptr) return; //Cancel

	std::cout << "The course " << *course << " has been added." << std::endl << std::endl;
	system("pause");
	system("cls");

	
	struct option {
		int assignedNum;
		std::string description;
		option(std::string desc, int num = NULL) : description(desc), assignedNum(num) {}
	};
	option title("Set course title");
	option units("Set course units");
	option desc("Set course description");
	option prereq("Add prerequisite(s)");
	std::string enter0 = " or enter 0 to cancel";

	int userChoice;
	do {
		int count = 0;
		std::cout << "Add a Course" << std::endl << std::endl;
		std::cout << fullCourseInfo(course) << std::endl
			<< count++ << " - Return" << std::endl;
		if (course->getCourseTitle() == "") {
			title.assignedNum = count++;
			std::cout << title.assignedNum << " - " << title.description << std::endl;
		}
		else title.assignedNum = NULL;
		if (course->getUnits() == NULL) {
			units.assignedNum = count++;
			std::cout << units.assignedNum << " - " << units.description << std::endl;
		}
		else units.assignedNum = NULL;

		if (course->getDescription() == "") {
			desc.assignedNum = count++;
			std::cout << desc.assignedNum << " - " << desc.description << std::endl;
		}
		else desc.assignedNum = NULL;

		prereq.assignedNum = count++;
		std::cout << prereq.assignedNum << " - " << prereq.description << std::endl;

		std::cout << std::endl;

		std::cout << "Enter a choice: ";
		userChoice = readIntInput();
		std::cout << std::endl;

		std::string temp;
		if (userChoice == 0) { break; }
		else if (userChoice == title.assignedNum) {
			std::cout << title.description << enter0 << ": ";
			std::getline(std::cin, temp);
			if (temp != std::to_string(0)) {
				course->setCourseTitle(temp);
				std::cout << std::endl << "Course title added." << std::endl << std::endl;
				system("pause");
			}
		}
		else if (userChoice == units.assignedNum) {
			std::cout << units.description << enter0 << ": ";
			int num = enterInteger();
			if (num > 0) {
				course->setUnits(num);
				std::cout << std::endl << "Course units added." << std::endl << std::endl;
				system("pause");
			}
		}
		else if (userChoice == desc.assignedNum) {
			std::cout << desc.description << enter0 << ": ";
			std::getline(std::cin, temp);
			if (temp != std::to_string(0)) {
				course->setDescription(temp);
				std::cout << std::endl << "Course description added." << std::endl << std::endl;
				system("pause");
			}
		}
		else if (userChoice == prereq.assignedNum) {
			printAllCourses();
			std::cout << std::endl;

			std::vector<CourseModule*> prerecVect = inputValidCourses();

			if (!prerecVect.empty()) {
				addMultiplePrereqs(course, prerecVect);
				std::cout << std::endl << "Course prerequisite(s) added." << std::endl << std::endl;
				system("pause");
			}
		}
		else {
			std::cout << std::endl << "Invalid choice." << std::endl << std::endl;
			system("pause");
		}
		system("cls");
	} while (userChoice != 0);
}

void Menu::subMenuCourseRemove() {
	if (Courses.empty()) {
		std::cout << "There are currently no courses entered. Returning to main menu" << std::endl << std::endl;
		system("pause");
		return;
	}
	std::cout << "Remove a Course" << std::endl << std::endl;
	printAllCourses();
	std::cout << std::endl;

	CourseModule *course = inputValidCourse();
	if (course == nullptr) return;
	if (std::find(Courses.begin(), Courses.end(), course) == Courses.end()) {
		std::cout << "The course you entered has not been added and therefore cannot be deleted." << std::endl << std::endl;
		system("pause");
		system("cls");
		return;
	}

	system("cls");

	std::string input;
	std::cout << "Remove a Course" << std::endl << std::endl 
		<< "Are you sure you want to delete " << course->getCourseSubject() << " " << course->getCourseNumber()
		<< "? (y/n) ";
	std::getline(std::cin, input);
	while (input != "Y" && input != "y" && input != "N" && input != "n") {
		std::cout << "Input invalid. Remove course? (y/n) ";
		std::getline(std::cin, input);
		std::cout << std::endl;
	}

	if (input == "Y" || input == "y") {
		removeCourse(course);
		std::cout << "The course has been removed." << std::endl;
		system("pause");
	}
	else return;
}

bool Menu::hasOneSpace(std::string str) {
	bool hasSpace = false;
	for (int i = 0; i < str.length(); ++i) {
		if (hasSpace && str[i] == ' ') return true;

		if (str[i] == ' ' && !hasSpace)
			hasSpace = true;
	}
	return hasSpace;
}

bool Menu::allInts(std::string str) {
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] < '0' || str[i] > '9') return false;
	}
	return true;
}

int Menu::enterInteger() {
	bool recheck;
	std::string input;
	do {
		recheck = false;
		std::getline(std::cin, input);

		for (int i = 0; i < input.length(); ++i) {
			if (input[i] < '0' || input[i] > '9') recheck = true;
		}

		if (recheck) {
			std::cout << "Invalid input. Enter an integer: ";
		}

	} while (recheck);
	return std::stoi(input);
}

CourseModule *Menu::inputValidCourse() {
	std::cout << "Enter course (ex. CECS 100) or enter 0 to cancel: ";
	std::string input = CourseInputChecker::getCourses();
	if (input == "") return nullptr;

	int num;
	std::string sub = input.substr(0, input.find_first_of("1234567890") - 1);
	input = input.substr(input.find_first_of("1234567890"), std::string::npos);
	if (input.find_first_of(' ') == std::string::npos) {
		num = std::stoi(input);
		input = "";
	}
	else {
		num = std::stoi(input.substr(0, input.find_first_of(' ')));
	}
	return courseSearch(sub, num);

}

CourseModule *Menu::inputNewValidCourse() {
	std::cout << "Enter course (ex. CECS 100) or enter 0 to cancel: ";
	std::string input = CourseInputChecker::getCourses();
	if (input == "") return nullptr;

	int num;
	std::string sub = input.substr(0, input.find_first_of("1234567890") - 1);
	input = input.substr(input.find_first_of("1234567890"), std::string::npos);
	if (input.find_first_of(' ') == std::string::npos) {
		num = std::stoi(input);
		input = "";
	}
	else {
		num = std::stoi(input.substr(0, input.find_first_of(' ')));
	}
	return addCourse(sub, num);

}

std::vector<CourseModule*> Menu::inputValidCourses() {
	std::vector<CourseModule*> result;

	std::cout << "Enter course(s) (separated by spaces; ex: CECS 100 CECS 101) or enter 0 to cancel : ";
	std::string input = CourseInputChecker::getCourses();

	std::string sub;
	int num;
	while (input != "") {
		sub = input.substr(0, input.find_first_of("1234567890") - 1);
		input = input.substr(input.find_first_of("1234567890"), std::string::npos);
		if (input.find_first_of(' ') == std::string::npos) {
			num = std::stoi(input);
			input = "";
		}
		else {
			num = std::stoi(input.substr(0, input.find_first_of(' ')));
			input = input.substr(input.find_first_of(' '), std::string::npos);
		}

		result.push_back(addCourse(sub, num));
	}

	return result;
}

void Menu::subMenuCourseEdit() {
	if (Courses.empty()) {
		std::cout << "There are currently no courses entered. Returning to main menu" << std::endl << std::endl;
		system("pause");
		return;
	}

	std::cout << "Edit a course" << std::endl << std::endl;
	printAllCourses();
	std::cout << std::endl;
	CourseModule* course = inputValidCourse();
	if (course == nullptr) return;
	if(std::find(Courses.begin(), Courses.end(), course) == Courses.end()) {
		std::cout << "The course you entered has not been added and therefore cannot be edited." << std::endl << std::endl;
		system("pause");
		system("cls");
		return;
	}

	system("cls");

	std::string input;
	int userChoice;
	do {
		std::cout << "Edit a Course" << std::endl << std::endl;
		std::cout << fullCourseInfo(course) << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Edit course title" << std::endl
			<< "2 - Edit course units" << std::endl
			<< "3 - Edit course description" << std::endl
			<< "4 - Edit course prerequisites" << std::endl << std::endl
			<< "Enter a choice: ";
		userChoice = readIntInput();
		std::cout << std::endl;
		switch (userChoice) {
			int choice;
		case 0:
			break;
		case 1:
			if (course->getCourseTitle() != "")
				std::cout << "Current course title: " << course->getCourseTitle() << std::endl << std::endl;

			std::cout << "Enter new course title or enter 0 to cancel: ";
			std::getline(std::cin, input);
			if (input == "0") break;

			course->setCourseTitle(input);
			std::cout << "Course title changed." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 2: {
			if (course->getUnits() != NULL)
				std::cout << "Current course units: " << course->getUnits() << std::endl << std::endl;

			std::cout << "Enter new course units or enter 0 to cancel: ";

			int num = enterInteger();

			if (num == 0) break;

			course->setUnits(num);
			std::cout << "Course units changed." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		}
		case 3:
			if (course->getDescription() != "")
				std::cout << "Current course description: " << course->getDescription() << std::endl << std::endl;

			std::cout << "Enter new course description or enter 0 to cancel: ";

			std::getline(std::cin, input);
			if (input == "0") break;

			course->setDescription(input);
			std::cout << "Course description changed." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 4: {
			do {
				system("cls");
				std::cout << "Edit Prerequisites" << std::endl << std::endl
					<< "Current prerequisites: " << course->getPrereqsAsString() << std::endl << std::endl
					<< "0 - Return" << std::endl
					<< "1 - Add prerequisite" << std::endl
					<< "2 - Remove prerequisite" << std::endl << std::endl
					<< "Enter choice: ";

				choice = readIntInput();
				std::cout << std::endl;

				std::string temp;
				switch (choice) {
				case 0:
					break;
				case 1:
					printAllCourses();
					std::cout << std::endl;
					std::cout << "Enter course(s) (separated by spaces; ex: CECS 100 CECS 101) or enter 0 to cancel: ";
					std::getline(std::cin, temp);
					if (temp != std::to_string(0)) {
						readPrerequisites(course, temp);
						std::cout << "Course prerequisite(s) added." << std::endl;
						system("pause");
					}
					break;
				case 2: {
					std::cout << fullCourseInfo(course) << std::endl;
					std::vector<CourseModule*> prereqCourses = inputValidCourses();

					if (prereqCourses.empty()) break;

					for (std::vector<CourseModule*>::iterator it = prereqCourses.begin(); it != prereqCourses.end(); ++it) {
						course->removePrerequisite(*it);
					}

					std::cout << "Course(s) has been removed from prerequisites." << std::endl << std::endl;
					system("pause");
					break;
				}
				default:
					std::cout << "Invalid choice." << std::endl << std::endl;
					system("pause");
					break;
				}
				system("cls");
			} while (choice != 0);
			break;
		}
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			system("pause");
			break;
		}
		system("cls");
	} while (userChoice != 0);
}

CourseModule* Menu::addCourse(std::string sub, int num) {
	CourseModule* toBeAdded = courseSearch(sub, num);
	if (toBeAdded == nullptr) {
		toBeAdded = new CourseModule(sub, num);
		Courses.push_back(toBeAdded);
	}
	return toBeAdded;
}

CourseModule* Menu::addCourse(CourseModule* course) {
	CourseModule* toBeAdded = courseSearch(course->getCourseSubject(), course->getCourseNumber());
	if (toBeAdded == nullptr) {
		Courses.push_back(course);
	}
	return course;
}

void Menu::addMultiplePrereqs(CourseModule* course, const std::vector<CourseModule*>& prereqs) {
	if (prereqs.empty()) return;
	for (std::vector<CourseModule*>::const_iterator it = prereqs.begin(); it != prereqs.end(); ++it) {
		course->addPrerequisite(*it);
	}
}

void Menu::removeCourse(CourseModule *course) {
	std::vector<CourseModule*>::iterator courseIt = std::find(Courses.begin(), Courses.end(), course);
	delete* courseIt; //deallocating memory and removing this course from any courses that have it as a prerequisite
	Courses.erase(courseIt); //removing the course's pointer from the list of courses
}
