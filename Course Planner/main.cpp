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
	CourseGraph test;
	CourseData::load(test, "course_data.txt");

	CourseGraph test2;
	CourseModule c1("MATH", 100, "intro math1");
	CourseModule c2("MATH", 101, "intro math2");
	CourseModule c3("Math", 122, "intro math3");
	test2.insert(c1);
	test2.insert(c2);
	test2.insert(c3);
	test2.addEdge(c1, c2);

	test.printCourses();
	cout << "------------------------------" << endl << endl;
	test2.printCourses();
	cout << "------------------------------" << endl << endl;
	test2.merge(test);
	test2.printCourses();
	cout << "------------------------------" << endl << endl;
	cout << test2.search(c3)->course.getUnits() << endl;
	cout << test.search(c3)->course.getUnits() << endl;
	cout << "------------------------------" << endl << endl;

	test.merge(test2);
	test.printCourses();

	cout << test2.search(c3)->course.getUnits() << endl;
	cout << test.search(c3)->course.getUnits() << endl;


	
	
	system("pause");
	return 0;

	Menu menu;
	menu.runMenu();

	return 0;
}