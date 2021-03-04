#include <iostream>
#include "Menu.h"

#include "CourseModule.h"//DEBUGGING
#include <string>
#include "CourseInputChecker.h"
#include "CourseGraph.h"
#include <set>

namespace std {
	static void pause() {
		std::cout << "Press enter to continue...";
		std::cin.get();
	}
}

using std::cout;
using std::endl;

int main() {
	CourseGraph graph;
	graph.insert(CourseModule("CECS", 100));
	graph.insert(CourseModule("CECS", 200));
	graph.insert(CourseModule("CECS", 101));
	graph.insert(CourseModule("CECS", 100));

	graph.printCourses();

	std::pause();
	return 0;

	Menu menu;
	menu.runMenu();

	return 0;
}