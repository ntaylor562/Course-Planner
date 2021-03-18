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

	Menu menu;
	menu.runMenu();

	return 0;
}