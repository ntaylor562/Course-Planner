#include <iostream>
#include "Menu.h"

#include "CourseModule.h"//DEBUGGING
#include <string>
#include "CourseInputChecker.h"
#include "CourseGraph.h"
#include "CourseData.h"
#include <vector>

namespace easy {
	static void pause() {
		std::cout << "Press enter to continue...";
		std::cin.get();
	}
}

using std::cout;
using std::endl;

int main() {

	//CourseGraph test;
	//test.insert(CourseModule("CECS 100"));
	//test.insert(CourseModule("CECS 200"));
	//test.insert(CourseModule("CECS 102"));
	//test.insert(CourseModule("CECS 101"));
	//test.insert(CourseModule("CECS 150"));
	//test.addEdge(CourseModule("CECS 228"), CourseModule("CECS 229"));
	//test.addEdge(CourseModule("CECS 101"), CourseModule("CECS 102"));

	//test.addEdge(CourseModule("CECS", 100), CourseModule("CECS", 101));
	//test.addEdge(CourseModule("CECS", 101), CourseModule("CECS", 102));
	//test.addEdge(CourseModule("CECS", 102), CourseModule("CECS", 103));
	//test.addEdge(CourseModule("CECS", 103), CourseModule("CECS", 104));
	//test.addEdge(CourseModule("CECS", 104), CourseModule("CECS", 105));

	////CourseData::load(test, "test.txt");

	//test.printCourses();

	//test.removeEdge(CourseModule("CECS", 101), CourseModule("CECS", 102));

	//test.remove(CourseModule("CECS 229"));

	//test.printCourses();

	//easy::pause();
	//CourseData::store(test, "test.txt");

	//return 0;

	Menu menu;
	menu.runMenu();

	return 0;
}