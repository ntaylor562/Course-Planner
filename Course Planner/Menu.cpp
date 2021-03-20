#include <iostream>
#include <vector>
#include <list>
#include <iomanip>
#include <algorithm>
#include "Menu.h"
#include "InputChecker.h"
#include "CourseInputChecker.h"
#include "CourseData.h"
#include "SchedulePreferenceData.h"

Menu::Menu() {

}

void Menu::setupMenu() {
	loadMajors();
	
	Major *selectedMajor;

	int userChoice = 0;

	//Loop until user enters valid choice
	while (1) {
		std::cout << "Select your major: " << std::endl;

		std::cout << "0 - None" << std::endl;
		int count = 1;
		for (const auto &i : majorList) {
			std::cout << count++ << " - " << i.getMajor() << " - " << i.getTitle() << std::endl;
		}

		std::cout << "Work in progress..." << std::endl << std::endl
			<< "Enter a choice: ";

		userChoice = InputChecker::getInt("Input invalid. Enter a choice: ");

		std::cout << std::endl << std::endl;

		if (userChoice == 0) {
			return;
		}
		else if (1 <= userChoice && userChoice <= majorList.size()) {
			std::list<Major>::iterator it = majorList.begin();
			for (int i = 0; i < userChoice; ++i)
				++it;
			selectedMajor = &*(--it);
			break;
		}
		else {
			std::cout << "Invalid choice." << std::endl << std::endl;
			system("pause");
			system("cls");
		}
	}

	Courses.merge(selectedMajor->getMajorReq());

	for (const auto &e : selectedMajor->getElectives()) {
		std::cout << "You must choose " << e.unitMinimum << " units out of these courses" << std::endl << std::endl;
		int count = 1;
		for (const auto &v : e.electives) {
			std::cout << count++ << " - " << v->course << " - " << v->course.getCourseTitle() << " (" << v->course.getUnits() << ")" << std::endl;
		}

		std::cout << std::endl << "Enter a choice: ";
		userChoice = InputChecker::getIntRange(1, e.electives.size(), "Input invalid. Enter a choice: ");
		std::cout << std::endl;

		std::list<vertex *>::const_iterator it = e.electives.begin();
		for (int i = 0; i < userChoice; ++i) //Finding the course the user selected
			++it;
		//Inserting the vertex into the graph
		Courses.insert(**(--it));

		std::cout << std::endl << std::endl;
	}


	for (const auto &choiceList : selectedMajor->getChoiceCourses()) {
		std::cout << "You must choose one of these courses" << std::endl;
		int count = 1;
		for(const auto &v : choiceList) {
			std::cout << count++ << " - " << v->course << v->course.getCourseTitle() << std::endl;
		}

		std::cout << std::endl << "Enter a choice: ";
		userChoice = InputChecker::getIntRange(1, choiceList.size(), "Input invalid. Enter a choice: ");

		std::list<vertex *>::const_iterator it = choiceList.begin();
		for (int i = 0; i < userChoice; ++i)
			++it;

		Courses.insert(**--it);

		std::cout << std::endl << std::endl;
	}
	std::cout << "A list of courses has been generated for you. Check to make sure these courses are the ones " << std::endl
		<< "you need. MAKE SURE to save your courses or you will need to set them up again." << std::endl << std::endl;

	system("pause");
	system("cls");
}

void Menu::readPrerequisites(CourseModule &course, std::string str) {
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

		Courses.addEdge(CourseModule(sub, num), course);
	} while (str.find(' ') != std::string::npos);
}

void Menu::runMenu() {
	std::cout << "Welcome to Course Planner" << std::endl
		<< "This application allows you to add a list of courses and their information" << std::endl
		<< "to help manage all the classes you need to take." << std::endl << std::endl;

	loadSettings();

	try {
		SchedulePreferenceData::load(CourseScheduler, Courses, scheduleDataFile);
	}
	catch (std::runtime_error x) {
		std::cout << x.what() << std::endl
			<< "Save schedule preference data to create a file. Schedule preferences set to default." << std::endl;
		CourseScheduler = Scheduler(Courses);
		system("pause");
		system("cls");
	}

	std::ifstream courseData;
	courseData.open(dataPath + courseDataFileName);
	if (!courseData.is_open()) { //Checking if a course data file has been saved. If not, we need to set things up
		setupMenu();
	}
	else if (courseData.peek() == EOF) { //File is empty so we go to setup
		setupMenu(); //This is in a different if statement because we can't peek a file that can't open
	}
	else { //If the user's course data exists, load it into the user's course graph
		try {
			CourseData::load(Courses, courseDataFileName);
		}
		catch (std::runtime_error x) {
			std::cout << x.what() << std::endl
				<< "Save courses to create a working file. List of courses is empty." << std::endl;
			system("pause");
			system("cls");
		}
	}
	courseData.close();

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
	while (!exit) {
		std::cout << "Main Menu" << std::endl
			<< "0 - Exit" << std::endl
			<< "1 - Show courses" << std::endl
			<< "2 - Add a course" << std::endl
			<< "3 - Edit an existing course" << std::endl
			<< "4 - Remove a course" << std::endl
			<< "5 - Search courses" << std::endl
			<< "6 - Generate schedule" << std::endl
			<< "7 - Save courses" << std::endl
			<< "8 - Settings" << std::endl << std::endl;

		std::cout << "Enter a choice: ";
		int userChoice = InputChecker::getInt();
		switch (userChoice) {
		case 0: //Exit
			exit = true;
			break;
		case 1: //Print courses
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
		case 2: //Add course
			system("cls");
			subMenuCourseAdd();
			break;
		case 3: //Edit course
			system("cls");
			subMenuCourseEdit();
			break;
		case 4: //Remove course
			system("cls");
			subMenuCourseRemove();
			break;
		case 5: //Search courses
			system("cls");
			subMenuCourseSearch();
			break;
		case 6: //Generate schedule
			system("cls");
			subMenuCourseScheduler();
			break;
		case 7: //Save courses
			CourseData::store(Courses, courseDataFileName);
			std::cout << "Data has been saved to " << courseDataFileName << std::endl << std::endl;
			system("pause");
			break;
		case 8:
			system("cls");
			subMenuSettings();
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			system("pause");
			break;
		}
		system("cls");
		if(autoSave) CourseData::store(Courses, courseDataFileName);
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

	int userChoice = 0;
	do {
		int count = 0;
		std::cout << "Add Course" << std::endl << std::endl;
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
	std::cout << "Remove Course" << std::endl << std::endl;
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

//Searches linearly through the course data (subject, number, title, and description)
void Menu::subMenuCourseSearch() {
	std::cout << "Search Courses" << std::endl << std::endl
		<< "Search for something or enter 0 to exit: ";
	std::string input;
	std::getline(std::cin, input);

	//Trim the input in case there are leading or trailing spaces
	input = input.substr(input.find_first_not_of(" "), std::string::npos);
	input = input.substr(0, input.find_last_not_of(" ") + 1);

	if (input == "0") {
		system("cls");
		return;
	}

	for (auto &c : input) { //Making input lowercase
		c = std::tolower(c);
	}

	//TODO Maybe add a fast search if the search input is in the format SUBJECT NUM (Ex. CECS 100)
	std::list<std::string> searchData;
	
	for (auto it : Courses) { //Initializing searchData
		searchData.push_back("");
	}

	std::list<std::string>::iterator dataIt = searchData.begin();
	for (auto it = Courses.begin(); it != Courses.end(); ++it) { //Populating searchData
		*dataIt += (*it)->course.getCourseSubject() + " ";
		*dataIt += std::to_string((*it)->course.getCourseNumber());
		*dataIt += (*it)->course.getCourseTitle();
		*dataIt += (*it)->course.getDescription();

		for (auto &c : *dataIt) {
			c = std::tolower(c);
		}

		++dataIt;
	}

	bool found = false;

	dataIt = searchData.begin();
	for (auto it = Courses.begin(); it != Courses.end(); ++it) { //Searching through data

		if (dataIt->find(input) != std::string::npos) { //Found input in the data for a course
			std::cout << fullCourseInfo((*it)->course) << std::endl
				<< "Is this the course you're looking for? (y/n) ";

			if (!InputChecker::getBool()) { //Not the course so move on to next search result
				++dataIt;
				continue; 
			}

			//Found course
			system("cls");
			editCourse((*it)->course);
			found = true;
			break;
		}

		++dataIt;
	}

	if (!found) {
		std::cout << "Your search returned no results. Try entering the course subject or number, or part of the title or description." << std::endl << std::endl;
		system("pause");
	}

	system("cls");
}

void Menu::subMenuCourseScheduler() {
	int userChoice = 0;
	updateScheduler();
	do {
		std::cout << "Course Scheduler" << std::endl << std::endl;
		
		if (!CourseScheduler.getCompletedCourses().empty()) {
			std::cout << "Completed courses:";
			for (const auto &i : CourseScheduler.getCompletedCourses()) {
				std::cout << " " << i;
			}
			std::cout << std::endl << std::endl;
		}
		

		std::cout << "0 - Return" << std::endl
			<< "1 - Generate a schedule" << std::endl
			<< "2 - Mark course as complete" << std::endl
			<< "3 - Mark course as incomplete" << std::endl
			<< "4 - Add a restriction" << std::endl
			<< "5 - Remove a restriction" << std::endl
			<< "6 - Save schedule preferences" << std::endl << std::endl
			<< "Enter a choice: ";

		userChoice = InputChecker::getInt();
		std::cout << std::endl;

		Semester s;
		std::vector<CourseModule *> enteredCoursesPointers;
		std::vector<CourseModule> enteredCourses;
		switch (userChoice) {
		case 0:
			break;
		case 1:
			s = enterSemester("Enter the semester you want to start the schedule with (Ex: Spring 2020): ");
			system("cls");
			printSchedule(s);
			system("pause");
			system("cls");
			break;
		case 2:
			printAllCourses();
			std::cout << std::endl << "Prerequisites of courses you enter will be assumed complete." << std::endl;
			enteredCoursesPointers = inputValidCourses();

			for (const auto &i : enteredCoursesPointers) {
				enteredCourses.push_back(*i);
			}
			CourseScheduler.complete(enteredCourses);

			if(!enteredCourses.empty()) std::cout << "Course(s) marked complete." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 3:
			printAllCourses();
			std::cout << std::endl;
			enteredCoursesPointers = inputValidCourses();

			for (const auto &i : enteredCoursesPointers) {
				enteredCourses.push_back(*i);
			}
			CourseScheduler.uncomplete(enteredCourses);

			if (!enteredCourses.empty()) std::cout << "Course(s) marked incomplete." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 4:
			system("cls");
			addRestriction();
			break;
		case 5:
			system("cls");
			removeRestriction();
			break;
		case 6:
			SchedulePreferenceData::store(CourseScheduler, scheduleDataFile);
			std::cout << "Schedule preferences have been saved." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			break;
		}
	} while (userChoice != 0);
}

void Menu::subMenuSettings() {
	int userChoice = 0;
	do {
		std::cout << "Settings" << std::endl << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Toggle Autosave (" << ((autoSave) ? "ON" : "OFF") << ")" << std::endl
			<< "2 - Change user course data file name" << std::endl
			<< "3 - Choose another major" << std::endl
			<< "4 - Reset all user course data" << std::endl << std::endl
			<< "Enter a choice: ";

		userChoice = InputChecker::getInt("Input invalid. Enter a choice: ");
		std::cout << std::endl;

		switch (userChoice) {
		case 1: //Toggle autosave
			autoSave = !autoSave;
			std::cout << "Autosave is now " << ((autoSave) ? "on" : "off") << "." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 2:

			std::cout << "If you have saved a file with the previous name, it will not be deleted." << std::endl
				<< "Enter the file name you would like to save your courses to: ";
			courseDataFileName = InputChecker::getString();
			std::cout << "Course Planner will now save your courses to \"" << courseDataFileName << "\"." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 3:
			std::cout << "If you're not double majoring, you should reset your course data. Continue? (y/n) ";
			if (!InputChecker::getBool("Input invalid. Enter y or n: ")) {
				system("cls");
				break;
			}

			std::cout << std::endl << "Reset course data? (y/n) ";
			if (InputChecker::getBool("Input invalid. Enter y or n: ")) {
				std::cout << "All courses have been wiped. If this was a mistake, DO NOT SAVE" << std::endl << std::endl;
			}
			setupMenu();
			break;
		case 4:
			std::cout << "Are you sure you want to remove all course data?" << std::endl
				<< "This will not delete your data unless you save or if autosave is enabled." << std::endl
				<< "(y/n) ";
			if (InputChecker::getBool("Input Invalid. Enter y or n: ")) {
				Courses = CourseGraph();
				std::cout << "All courses have been wiped. If this was a mistake, DO NOT SAVE" << std::endl << std::endl;
			}
			else std::cout << "Cancelled." << std::endl << std::endl;

			system("pause");
			system("cls");
			break;
		default:
			std::cout << "Invalid choice." << std::endl;
			system("pause");
			system("cls");
			break;
		}
		saveSettings();

	} while (userChoice != 0);
}

void Menu::subMenuMajorCreator() {
	int userChoice = 0;

	do {
		std::cout << "Major Creator" << std::endl << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Add a major" << std::endl
			<< "2 - Edit a major" << std::endl
			<< "3 - Remove a major" << std::endl << std::endl
			<< "Enter a choice: ";

		userChoice = InputChecker::getInt("Input invalid. Enter a choice: ");

		switch (userChoice) {
		case 0:
			system("cls");
			break;
		case 1:
			system("cls");
			subMenuAddMajor();
			break;
		case 2:

			break;
		case 3:

			break;
		default:
			std::cout << "Invalid choice" << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		}

	} while (userChoice != 0);

}

void Menu::subMenuAddMajor() {
	int userChoice = 0;

	std::cout << "Add a Major" << std::endl << std::endl
		<< "Enter the name of the major (Ex: Computer Science): ";
	std::string majorName = InputChecker::getLine();
	std::cout << std::endl << "Enter the major's acronym (Ex: CECS): ";
	std::string majorAcronym = InputChecker::getLine();
	std::cout << std::endl;

	//Trimming leading and trailing spaces if there are any
	majorName = majorName.substr(majorName.find_first_not_of(" "));
	majorName = majorName.substr(0, majorName.find_last_not_of(" ") + 1);
	majorAcronym = majorAcronym.substr(majorAcronym.find_first_not_of(" "));
	majorName = majorAcronym.substr(0, majorAcronym.find_last_not_of(" ") + 1);

	Major newMajor(majorAcronym, majorName);

	Menu majorMenu;

	majorMenu.courseDataFileName = newMajor.getMajor() + "_major_requirements.txt";
	majorMenu.autoSave = false;
	majorMenu.Courses = newMajor.getMajorReq();

	Menu electiveMenu;
	electiveMenu.courseDataFileName = newMajor.getMajor() + "_electives_data.txt";
	electiveMenu.autoSave = false;
	CourseData::load(electiveMenu.Courses, newMajor.getMajor() + "_electives_data.txt");

	Menu choiceCourseMenu;
	choiceCourseMenu.courseDataFileName = newMajor.getMajor() + "_choice_courses_data.txt";
	choiceCourseMenu.autoSave = false;
	CourseData::load(choiceCourseMenu.Courses, newMajor.getMajor() + "_choice_courses_data.txt");

	do {
		std::cout << "Add a Major" << std::endl << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Edit major requirements" << std::endl
			<< "2 - Edit elective groups" << std::endl
			<< "3 - Edit choice courses" << std::endl << std::endl;

		userChoice = InputChecker::getInt("Invalid input. Enter a choice: ");

		switch (userChoice) {
		case 0:
			break; 
		case 1: 										//Major requirements menu
			do {
				std::cout << "Major Requirements" << std::endl << std::endl
					<< "0 - Return" << std::endl
					<< "1 - Show courses" << std::endl
					<< "2 - Add a course" << std::endl
					<< "3 - Edit a course " << std::endl
					<< "4 - Remove a course" << std::endl
					<< "5 - Save major required courses" << std::endl << std::endl
					<< "Enter a choice: ";

				userChoice = InputChecker::getInt("Invalid input. Enter a choice: ");

				switch (userChoice) {
				case 0:
					system("cls");
					break;
				case 1:
					system("cls");

					if (majorMenu.Courses.empty()) {
						std::cout << "List of courses is empty." << std::endl << std::endl;
						system("pause");
						break;
					}

					majorMenu.printAllCourseData();
					std::cout << std::endl;
					majorMenu.printAllIncompleteDataCourses();
					std::cout << std::endl;
					majorMenu.printAllCourses();
					std::cout << std::endl;

					system("pause");
					system("cls");
					break;
				case 2:
					system("cls");
					majorMenu.subMenuCourseAdd();
					system("cls");
					break;
				case 3:
					system("cls");
					majorMenu.subMenuCourseEdit();
					system("cls");
					break;
				case 4:
					system("cls");
					majorMenu.subMenuCourseRemove();
					system("cls");
					break;
				case 5:
					CourseData::store(majorMenu.Courses, majorMenu.courseDataFileName);
					std::cout << "Data has been saved to " << majorMenu.courseDataFileName << std::endl << std::endl;
					system("pause");
					system("cls");
					break;
				default:
					std::cout << "Invalid choice." << std::endl << std::endl;
					system("pause");
					system("cls");
					break;
				}
			} while (userChoice != 0);
			break;
		case 2:										//Electives menu
			do {
				std::cout << "Electives" << std::endl << std::endl
					<< "0 - Return" << std::endl
					<< "1 - Show elective groups" << std::endl
					<< "2 - Add elective group" << std::endl << std::endl
					<< "Enter a choice: ";

				userChoice = InputChecker::getInt("Invalid input. Enter a choice: ");

				std::list<ElectiveGroup> groups = newMajor.getElectives();
				ElectiveGroup newGroup;
				switch (userChoice) {
				case 0:
					system("cls");
					break;
				case 1: //Show elective groups
					system("cls");
					for (const auto &e : groups) {
						int count = 1;
						std::cout << "Group " << count++ << std::endl
							<< "Minimum units a student must take in this group is " << e.unitMinimum << " units." << std::endl;
						for (const auto &v : e.electives) {
							std::cout << "\t" << v->course << " - " << v->course.getCourseTitle() << " (" << v->course.getUnits() << ")" << std::endl;
						}
						std::cout << std::endl;
					}
					std::cout << std::endl;
					system("pause");
					system("cls");
					break;
				case 2: //Add elective group
					system("cls");
					std::cout << "Enter the minimum number of units a student must take in this elective group or 0 to return: ";
					newGroup.unitMinimum = InputChecker::getInt();
					if (newGroup.unitMinimum <= 0) break;

					for (const auto &c : electiveMenu.inputValidCourses()) {
						vertex *v = *electiveMenu.Courses.insert(*c);
						system("cls");
						electiveMenu.editCourse(v->course);
						newGroup.electives.push_back(v);
					}
					newMajor.addElectiveGroup(newGroup);
					break;
				default:
					std::cout << "Invalid choice." << std::endl << std::endl;
					system("pause");
					system("cls");
					break;
				}
			} while (userChoice != 0);
			break;
		case 3:										//Choice courses menu
			do {
				std::cout << "Choice Courses" << std::endl << std::endl
					<< "0 - Return" << std::endl
					<< "1 - Show choice courses" << std::endl
					<< "2 - Add choice courses" << std::endl << std::endl
					<< "Enter a choice: ";

				userChoice = InputChecker::getInt("Invalid input. Enter a choice: ");

				switch (userChoice) {
				case 0:
					system("cls");
					break;
				case 1:
					system("cls");
					for (const auto &lst : newMajor.getChoiceCourses()) {
						std::cout << "User must choose one of the following courses" << std::endl;
						for (const auto &v : lst) {
							std::cout << "\t" << v->course << " - " << v->course.getCourseTitle() << " (" << v->course.getUnits() << ")" << std::endl;
						}
						std::cout << std::endl;
					}
					std::cout << std::endl;
					system("pause");
					system("cls");
					break;
				case 2:
					
					break;
				default:
					std::cout << "Invalid choice." << std::endl << std::endl;
					system("pause");
					system("cls");
					break;
				}
			} while (userChoice != 0);
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		}

	} while (userChoice != 0);
}

void Menu::addRestriction() {
	int userChoice = 0;

	do {
		std::cout << "Add a Restriction" << std::endl << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Restrict a course for a specific semester" << std::endl
			<< "2 - Restrict a specific semester" << std::endl
			<< "3 - Set an overall unit limit" << std::endl
			<< "4 - Set a unit limit for a specific semester" << std::endl;

		std::string seasons[] = { "Winter", "Spring", "Summer", "Fall" };
		for (int i = 0; i < 4; ++i) {
			std::cout << i + 5 << " - ";
			if (CourseScheduler.getRestrictedSeasons().empty()) std::cout << "Restrict ";
			else {
				bool found = false;
				for (const auto &it : CourseScheduler.getRestrictedSeasons()) {
					if (it == static_cast<Semester::Seasons>(i)) {
						std::cout << "Allow ";
						found = true;
						break;
					}
				}
				if (!found) std::cout << "Restrict ";
			}
			std::cout << seasons[i] << " semester" << std::endl;
		}
		std::cout << std::endl << "Enter a choice: ";

		userChoice = InputChecker::getInt();
		std::cout << std::endl;

		int lim;
		Semester s;
		std::string input;
		CourseModule *course;
		switch (userChoice) {
		case 0:
			system("cls");
			break;
		case 1:
			printAllCourses();
			std::cout << std::endl;
			course = inputValidCourse();
			if (course == nullptr) {
				break;
			}
			s = enterSemester();
			CourseScheduler.addRestriction(*course, s.season, s.year);

			std::cout << *course << " will no longer be allowed in " << seasons[static_cast<int>(s.season)] << " " << s.year << "." << std::endl << std::endl;
			break;
		case 2:
			s = enterSemester();
			CourseScheduler.addRestriction(s);
			std::cout << seasons[static_cast<int>(s.season)] << " " << s.year << " is now restricted from having courses." << std::endl << std::endl;
			break;
		case 3:
			std::cout << "What would you like the unit limit for all semesters to be (0 for default)? ";
			lim = InputChecker::getIntRange(0, 21, "Input invalid. Limit must be a number from 0 to 21. ");
			CourseScheduler.setUnitLimit(lim);
			std::cout << std::endl << "Overall unit limit is now " << CourseScheduler.getMaxUnits() << "." << std::endl << std::endl;
			break;
		case 4:
			s = enterSemester();
			std::cout << "What would you like the unit limit for " << s.ToString() << " to be (0 for default)? ";
			lim = InputChecker::getIntRange(0, 21, "Input invalid. Limit must be a number from 0 to 21. ");
			CourseScheduler.setSemesterUnitLimit(s, lim);
			std::cout << std::endl << s.ToString() << " unit limit is now ";
			if (lim == 0) std::cout << "0." << std::endl << std::endl;
			else std::cout << ((lim >= 4) ? lim : 4) << "." << std::endl << std::endl;
			break;
		case 5:
			CourseScheduler.setWinterAllowed(!CourseScheduler.getWinterAllowed());
			std::cout << "Winter is now " << ((CourseScheduler.getWinterAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		case 6:
			CourseScheduler.setSpringAllowed(!CourseScheduler.getSpringAllowed());
			std::cout << "Spring is now " << ((CourseScheduler.getSpringAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		case 7:
			CourseScheduler.setSummerAllowed(!CourseScheduler.getSummerAllowed());
			std::cout << "Summer is now " << ((CourseScheduler.getSummerAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		case 8:
			CourseScheduler.setFallAllowed(!CourseScheduler.getFallAllowed());
			std::cout << "Fall is now " << ((CourseScheduler.getFallAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			break;
		}
		if (userChoice != 0) {
			system("pause");
			system("cls");
		}
	} while (userChoice != 0);
}

void Menu::removeRestriction() {
	int userChoice = 0;;

	do {
		std::cout << "Remove a Restriction" << std::endl << std::endl;
		printScheduleRestrictions();
		std::cout << "0 - Return" << std::endl
			<< "1 - Remove a restriction on a course for a specific semester" << std::endl
			<< "2 - Remove a restriction on a specific semester" << std::endl
			<< "3 - Set an overall unit limit" << std::endl
			<< "4 - Set a unit limit for a specific semester" << std::endl;

		std::string seasons[] = { "Winter", "Spring", "Summer", "Fall" };
		for (int i = 0; i < 4; ++i) {
			std::cout << i + 5 << " - ";
			if (CourseScheduler.getRestrictedSeasons().empty()) std::cout << "Restrict ";
			else {
				bool found = false;
				for (const auto &it : CourseScheduler.getRestrictedSeasons()) {
					if (it == static_cast<Semester::Seasons>(i)) {
						std::cout << "Allow ";
						found = true;
						break;
					}
				}
				if (!found) std::cout << "Restrict ";
			}
			std::cout << seasons[i] << " semester" << std::endl;
		}
		std::cout << std::endl << "Enter a choice: ";

		int userChoice = InputChecker::getInt();
		std::cout << std::endl;

		int lim;
		Semester s;
		std::string input;
		CourseModule *course;
		switch (userChoice) {
		case 0:
			system("cls");
			break;
		case 1:
			printAllCourses();
			std::cout << std::endl;
			course = inputValidCourse();
			if (course == nullptr) {
				break;
			}
			s = enterSemester();
			CourseScheduler.removeRestriction(*course, s.season, s.year);

			std::cout << *course << " is now allowed in " << seasons[static_cast<int>(s.season)] << " " << s.year << "." << std::endl << std::endl;
			break;
		case 2:
			s = enterSemester();
			CourseScheduler.removeRestriction(s.season, s.year);
			std::cout << seasons[static_cast<int>(s.season)] << " " << s.year << " is no longer restricted from having courses." << std::endl << std::endl;
			break;
		case 3:
			std::cout << "What would you like the unit limit for all semesters to be (0 for default)? ";
			lim = InputChecker::getIntRange(0, 21, "Input invalid. Limit must be a number from 0 to 21. ");
			CourseScheduler.setUnitLimit(lim);
			std::cout << std::endl << "Overall unit limit is now " << CourseScheduler.getMaxUnits() << "." << std::endl << std::endl;
			break;
		case 4:
			s = enterSemester();
			std::cout << "What would you like the unit limit for " << s.ToString() << " to be (0 for default)? ";
			lim = InputChecker::getIntRange(0, 21, "Input invalid. Limit must be a number from 0 to 21. ");
			CourseScheduler.setSemesterUnitLimit(s, lim);
			std::cout << std::endl << s.ToString() << " unit limit is now ";
			if (lim == 0) std::cout << "0." << std::endl << std::endl;
			else std::cout << ((lim >= 4) ? lim : 4) << "." << std::endl << std::endl;
			break;
		case 5:
			CourseScheduler.setWinterAllowed(!CourseScheduler.getWinterAllowed());
			std::cout << "Winter is now " << ((CourseScheduler.getWinterAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		case 6:
			CourseScheduler.setSpringAllowed(!CourseScheduler.getSpringAllowed());
			std::cout << "Spring is now " << ((CourseScheduler.getSpringAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		case 7:
			CourseScheduler.setSummerAllowed(!CourseScheduler.getSummerAllowed());
			std::cout << "Summer is now " << ((CourseScheduler.getSummerAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		case 8:
			CourseScheduler.setFallAllowed(!CourseScheduler.getFallAllowed());
			std::cout << "Fall is now " << ((CourseScheduler.getFallAllowed()) ? "allowed." : "restricted.") << std::endl << std::endl;
			break;
		default:
			std::cout << "Invalid choice." << std::endl << std::endl;
			break;
		}
		if (userChoice != 0) {
			system("pause");
			system("cls");
		}
	} while (userChoice != 0);
}

void Menu::printScheduleRestrictions() const {
	std::string output;

	std::vector<Semester::Seasons> resSeasons = CourseScheduler.getRestrictedSeasons();
	if (!resSeasons.empty()) {
		output += "\tSeason Restrictions\n";
		for (const auto &i : resSeasons) {
			if (i == Semester::Seasons::WINTER) output += "\t\tWinter is not allowed to have courses.\n";
			else if (i == Semester::Seasons::SPRING) output += "\t\tSpring is not allowed to have courses.\n";
			else if (i == Semester::Seasons::SUMMER) output += "\t\tSummer is not allowed to have courses.\n";
			else if (i == Semester::Seasons::FALL) output += "\t\tFall is not allowed to have courses.\n";
		}
		output += "\n";
	}

	std::vector<Semester> resSemesters = CourseScheduler.getRestrictedSemesters();
	if (!resSemesters.empty()) {
		output += "\tSemester Restrictions\n";
		for (const auto &i : resSemesters) {
			output += "\t\t" + i.ToString();

			if (i.semesterRestricted) {
				output += " is not allowed to have courses.\n";
				continue;
			}

			output += " restricted courses: ";
			for (const auto &c : i.restricted)
				output += c.ToString() + " ";
			output += "\n";
		}
		output += "\n";

		if (output != "")
			std::cout << "Schedule Restrictions:" << std::endl;
		else std::cout << "There are no restrictions on the schedule generator." << std::endl << std::endl;
		
		std::cout << output;
	}
}

void Menu::printSchedule(Semester s) const {
	Schedule courseSchedule = CourseScheduler.generateSchedule(s.season, s.year);

	if (Courses.empty()) {
		std::cout << "Course list is empty and we cannot generate a schedule with no courses." << std::endl << std::endl;
		return;
	}
	std::cout << "Course Schedule" << std::endl << std::endl;
	for (const auto &sem : courseSchedule.CoursePlan) { //For every semester
		std::cout << sem.ToString() << ":";
		for (const auto &c : sem.courses) {
			std::cout << "\n\t" << c << " - " << c.getCourseTitle() << " (" << c.getUnits() << ")";
		}
		std::cout << std::endl << std::endl;
	}

}

void Menu::editCourse(CourseModule &c) {
	std::string input;
	int userChoice = 0;
	do {
		std::cout << "Edit Course" << std::endl << std::endl;
		std::cout << fullCourseInfo(c) << std::endl
			<< "0 - Return" << std::endl
			<< "1 - Edit course title" << std::endl
			<< "2 - Edit course units" << std::endl
			<< "3 - Edit course description" << std::endl
			<< "4 - Edit course prerequisites" << std::endl << std::endl
			<< "Enter a choice: ";
		userChoice = InputChecker::getInt();
		std::cout << std::endl;
		switch (userChoice) {
		case 0:
			break;
		case 1:
			if (c.getCourseTitle() != "")
				std::cout << "Current course title: " << c.getCourseTitle() << std::endl << std::endl;

			std::cout << "Enter new course title or enter 0 to cancel: ";
			std::getline(std::cin, input);
			if (input == "0") break;

			c.setCourseTitle(input);
			std::cout << "Course title changed." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 2: {
			if (c.getUnits() != NULL)
				std::cout << "Current course units: " << c.getUnits() << std::endl << std::endl;

			std::cout << "Enter new course units or enter 0 to cancel: ";

			int num = InputChecker::getInt();

			if (num == 0) break;

			c.setUnits(num);
			std::cout << "Course units changed." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		}
		case 3:
			if (c.getDescription() != "")
				std::cout << "Current course description: " << c.getDescription() << std::endl << std::endl;

			std::cout << "Enter new course description or enter 0 to cancel: ";

			std::getline(std::cin, input);
			if (input == "0") break;

			c.setDescription(input);
			std::cout << "Course description changed." << std::endl << std::endl;
			system("pause");
			system("cls");
			break;
		case 4: {
			int choice = 0;
			do {
				system("cls");
				std::cout << "Edit Prerequisites" << std::endl << std::endl
					<< "Current prerequisites: ";

				//Print prerequisites
				vertex *v = Courses.search(c);
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
						readPrerequisites(c, temp);
						std::cout << "Course prerequisite(s) added." << std::endl;
						system("pause");
					}
					break;
				case 2: {
					std::cout << fullCourseInfo(c) << std::endl;

					if (v->prerequisites.empty()) {
						std::cout << "Prerequisites are empty." << std::endl << std::endl;
						system("pause");
						break;
					}

					std::vector<CourseModule *> prereqCourses = inputValidCourses();

					if (prereqCourses.empty()) break;

					for (std::vector<CourseModule *>::iterator it = prereqCourses.begin(); it != prereqCourses.end(); ++it) {
						Courses.removeEdge(**it, c);
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

CourseModule *Menu::inputValidCourse() {
	std::cout << "Enter course (ex. CECS 100) or enter 0 to cancel: ";
	std::string input = CourseInputChecker::getCourses();
	if (input == "") return nullptr;

	int num;
	std::string sub = input.substr(0, input.find_first_of("1234567890") - 1);
	sub = sub.substr(sub.find_first_not_of(" "), std::string::npos);
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
	sub = sub.substr(sub.find_first_not_of(" "), std::string::npos);
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

Semester Menu::enterSemester(std::string message) const {
	Semester sem;

	std::cout << message;

	bool inputValid = false;
	while (!inputValid) {
		std::string input = InputChecker::getString();
		for (auto &i : input)
			i = std::tolower(i);

		if (input == "winter") sem.season = Semester::Seasons::WINTER;
		else if (input == "spring") sem.season = Semester::Seasons::SPRING;
		else if (input == "summer") sem.season = Semester::Seasons::SUMMER;
		else if (input == "fall") sem.season = Semester::Seasons::FALL;
		else {
			std::cout << "Invalid season. " << message;
			continue;
		}

		sem.year = InputChecker::getIntRange(1683, 9999999, "Invalid year. Enter a valid year: ");

		inputValid = true;
	}

	return sem;
}

void Menu::updateScheduler() {
	CourseScheduler.updateCourses(Courses);
}

void Menu::loadSettings() {
	std::ifstream inFile;
	inFile.open(settingsConfig);
	if (!inFile.is_open()) {
		std::cout << "Settings file not found. Settings set to default." << std::endl << std::endl;
		autoSave = false;
		courseDataFileName = "course_data.txt";
		saveSettings();
		system("pause");
		return;
	}

	std::string currentLine;

	while (!inFile.eof()) {
		std::getline(inFile, currentLine);
		if (currentLine == "") 
			return;
		if (currentLine.substr(0, currentLine.find_first_of(":")) == "Autosave") {
			autoSave = (currentLine[currentLine.size() - 1] == 'T');
		}
		else if (currentLine.substr(0, currentLine.find_first_of(":")) == "Course data file name") {
			courseDataFileName = currentLine.substr(currentLine.find_first_of(":") + 2);
		}
	}

	inFile.close();
}

void Menu::saveSettings() {
	std::ofstream outFile;
	outFile.open(settingsConfig);
	if (!outFile.is_open()) {
		std::cout << "Save failed. File cannot open." << std::endl << std::endl;
		system("pause");
		return;
	}

	outFile << "Autosave: " << ((autoSave) ? "T" : "F") << std::endl;
	outFile << "Course data file name: " << courseDataFileName;

	outFile.close();
}

void Menu::loadMajors() {

	try {
		majorList.push_back(Major("CECS", "Computer Science"));
	}
	catch (std::runtime_error x) {
		std::cout << x.what() << std::endl
			<< "Place the required file in the folder \"" << dataPath << "\b\" which should be in the same directory as the Course Planner executable file." << std::endl
			<< "If you don't have the file, find it at https://github.com/ntaylor562/Course-Planner." << std::endl;
		system("pause");
		system("cls");
	}

}

void Menu::saveMajors() {
}

std::vector<CourseModule*> Menu::inputValidCourses() {
	std::vector<CourseModule*> result;

	std::cout << "Enter course(s) (separated by spaces; ex: CECS 100 CECS 101) or enter 0 to cancel : ";
	std::string input = CourseInputChecker::getCourses();

	std::string sub;
	int num;
	while (input != "") {
		sub = input.substr(0, input.find_first_of("1234567890") - 1);
		sub = sub.substr(sub.find_first_not_of(" "), std::string::npos);
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

	std::cout << "Edit course" << std::endl << std::endl;
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
	editCourse(*course);
}

void Menu::addMultiplePrereqs(CourseModule* course, const std::vector<CourseModule*>& prereqs) {
	if (prereqs.empty()) return;
	for (std::vector<CourseModule*>::const_iterator it = prereqs.begin(); it != prereqs.end(); ++it) {
		Courses.addEdge(**it, *course);
	}
}
