#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "Menu.h"
#include "CourseInputChecker.h"
#include "CourseData.h"

Menu::Menu() {
	try {
		CourseData::load(Courses, dataFile);
	}
	catch (std::runtime_error x) {
		std::cout << x.what() << std::endl
			<< "List of courses is empty" << std::endl;
		system("pause");
		system("cls");
	}
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

		Courses.addEdge(CourseModule(sub, num), *course);
	} while (str.find(' ') != std::string::npos);
}

void Menu::runMenu() {
	std::cout << "Welcome to Course Planner" << std::endl
		<< "This application allows you to add a list of courses and their information" << std::endl
		<< "to help manage all the classes you need to take." << std::endl << std::endl;

	mainMenu();
}

void Menu::printAllCourses() {
	if (Courses.empty()) return;

	int spacing = 15;
	int coursesPerLine = 5;
	std::cout << std::left << "Current list of courses:\n\n";

	int count = 1;
	for (std::vector<vertex *>::const_iterator it = Courses.begin(); it != Courses.end(); ++it) {
		std::string course;
		if (!(*it)->course.dataComplete()) course += "~";
		course += (*it)->course.getCourseSubject() + " " + std::to_string((*it)->course.getCourseNumber());
		if (count <= coursesPerLine) {
			std::cout << std::setw(spacing) << course;
			++count;
		} else {
			std::cout << std::endl << std::setw(spacing) << course;
			count = 2;
		}
	}
	std::cout << std::endl;
}

void Menu::printAllIncompleteDataCourses() {
	if (Courses.empty()) return;

	std::vector<vertex *>::const_iterator it;
	bool empty = true;
	for (it = Courses.begin(); it != Courses.end(); ++it) {
		if (!(*it)->course.dataComplete()) {
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
	for (std::vector<vertex *>::const_iterator it = Courses.begin(); it != Courses.end(); ++it) {
		std::string course;
		if (!(*it)->course.dataComplete()) {
			course += "~";
			course += (*it)->course.getCourseSubject() + " " + std::to_string((*it)->course.getCourseNumber());
			if (count <= coursesPerLine) {
				std::cout << std::setw(distance) << course;
				++count;
			} else {
				std::cout << std::endl << std::setw(distance) << course;
				count = 2;
			}
		}
	}
	std::cout << std::endl;
}

void Menu::printAllCourseData() {
	if (Courses.empty()) return;

	std::cout << "Current list of courses and their data:\n\n";
	std::vector<vertex *>::const_iterator it;
	for (it = Courses.begin(); it != Courses.end() - 1; ++it) {
		std::cout << fullCourseInfo((*it)->course) << std::endl;
		std::cout << "-------------------------------------------------------------------------------" << std::endl << std::endl;
	}
	std::cout << fullCourseInfo((*(Courses.end() - 1))->course) << std::endl;
}

std::string Menu::fullCourseInfo(const CourseModule &course) {
	std::string result;
	if (!course.dataComplete()) result += "~";
	result += course.getCourseSubject() + " " + std::to_string(course.getCourseNumber());

	if (course.getCourseTitle() != "") result += " - " + course.getCourseTitle();
	if (course.getUnits() > 0) result += "\nUnits: " + std::to_string(course.getUnits());
	if (course.getDescription() != "") result += "\nDescription: " + course.getDescription();
	
	vertex *v = Courses.search(course);
	if (v != nullptr) {
		result += "\nPrerequisites: ";
		std::list<vertex *>::const_iterator it = v->prerequisites.begin();
		while (it != v->prerequisites.end()) {
			result += (*it)->course.getCourseSubject() + " " + std::to_string((*it)->course.getCourseNumber());
			++it;
			if (it != v->prerequisites.end()) result += ", ";
		}
	}

	result += "\n";
	return result;
}

void Menu::mainMenu() {
	bool exit = false;
	bool autoSave = false;
	while (!exit) {
		std::cout << "Main Menu" << std::endl
			<< "0 - Exit" << std::endl
			<< "1 - Show courses" << std::endl
			<< "2 - Add a course" << std::endl
			<< "3 - Remove a course" << std::endl
			<< "4 - Edit an existing course" << std::endl
			<< "5 - Save courses" << std::endl
			<< "6 - Toggle Autosave (" << ((autoSave) ? "ON" : "OFF") << ")" << std::endl;

		std::cout << "Enter a choice: ";
		int userChoice = InputChecker::getInt();
		switch (userChoice) {
		case 0:
			exit = true;
			break;
		case 1:
			system("cls");

			if (Courses.empty()) {
				std::cout << "List of courses is empty." << std::endl << std::endl;
				system("pause");
				break;
			}

			printAllCourseData();
			std::cout << std::endl;
			printAllIncompleteDataCourses();
			std::cout << std::endl;
			printAllCourses();
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
			CourseData::store(Courses, dataFile);
			std::cout << "Data has been saved to " << dataFile << std::endl << std::endl;
			system("pause");
			break;
		case 6: 
			autoSave = !autoSave;
			std::cout << "Autosave is now " << ((autoSave) ? "on" : "off") << "." << std::endl << std::endl;
			system("pause");
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			system("pause");
			break;
		}
		system("cls");
		if(autoSave) CourseData::store(Courses, dataFile);
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
		std::cout << fullCourseInfo(*course) << std::endl
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
		userChoice = InputChecker::getInt();
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
			int num = InputChecker::getInt();
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

	vertex *v = Courses.search(*course);
	if (v == nullptr) {
		std::cout << "The course you entered has not been added and therefore cannot be deleted." << std::endl << std::endl;
		system("pause");
		system("cls");
		return;
	}

	system("cls");

	std::string input;
	std::cout << "Remove a Course" << std::endl << std::endl;
	if (!v->prerequisiteFor.empty()) {
		std::cout << *course << " is a prerequisite for ";
		std::list<vertex *>::const_iterator it = v->prerequisiteFor.begin();
		while (it != v->prerequisiteFor.end()) {
			std::cout << (*it)->course;
			++it;
			if (it != v->prerequisiteFor.end()) std::cout << ", ";
		}

		std::cout << std::endl;
	}

		std::cout << "Are you sure you want to delete " << course->getCourseSubject() << " " << course->getCourseNumber()
		<< "? (y/n) ";
	std::getline(std::cin, input);
	while (input != "Y" && input != "y" && input != "N" && input != "n") {
		std::cout << "Input invalid. Remove course? (y/n) ";
		std::getline(std::cin, input);
		std::cout << std::endl;
	}

	if (input == "Y" || input == "y") {
		Courses.remove(*course);
		std::cout << "The course has been removed." << std::endl;
		system("pause");
	}
	else return;
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
	return &Courses.search(CourseModule(sub, num))->course;

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
	return &(*Courses.insert(CourseModule(sub, num)))->course;

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

		result.push_back(&(*Courses.insert(CourseModule(sub, num)))->course);
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
	if(Courses.search(*course) == nullptr) {
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
		std::cout << fullCourseInfo(*course) << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Edit course title" << std::endl
			<< "2 - Edit course units" << std::endl
			<< "3 - Edit course description" << std::endl
			<< "4 - Edit course prerequisites" << std::endl << std::endl
			<< "Enter a choice: ";
		userChoice = InputChecker::getInt();
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

			int num = InputChecker::getInt();

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
					<< "Current prerequisites: ";

				//Print prerequisites
				vertex *v = Courses.search(*course);
				std::list<vertex *>::const_iterator it = v->prerequisites.begin();
				while (it != v->prerequisites.end()) {
					std::cout << (*it)->course;
					++it;
					if (it != v->prerequisites.end()) std::cout << ", ";
				}

				std::cout << std::endl << std::endl;

				std::cout << "0 - Return" << std::endl
					<< "1 - Add prerequisite" << std::endl
					<< "2 - Remove prerequisite" << std::endl << std::endl
					<< "Enter choice: ";

				choice = InputChecker::getInt();
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
					std::cout << fullCourseInfo(*course) << std::endl;
					
					if (v->prerequisites.empty()) {
						std::cout << "Prerequisites are empty." << std::endl << std::endl;
						system("pause");
						break;
					}

					std::vector<CourseModule*> prereqCourses = inputValidCourses();

					if (prereqCourses.empty()) break;

					for (std::vector<CourseModule*>::iterator it = prereqCourses.begin(); it != prereqCourses.end(); ++it) {
						Courses.removeEdge(**it, *course);
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

void Menu::addMultiplePrereqs(CourseModule* course, const std::vector<CourseModule*>& prereqs) {
	if (prereqs.empty()) return;
	for (std::vector<CourseModule*>::const_iterator it = prereqs.begin(); it != prereqs.end(); ++it) {
		Courses.addEdge(**it, *course);
	}
}
