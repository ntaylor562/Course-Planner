#include <iostream>
#include <iomanip>
#include "CourseGraph.h"

void CourseGraph::insert(CourseModule c) {
	vertices.insert(vertex{ c });
}

void CourseGraph::printCourses() const {
	if (vertices.empty()) return;
	int spacing = 15;
	std::cout << std::left;

	for (std::set<vertex>::iterator i = vertices.begin(); i != vertices.end(); ++i) {
		std::cout << std::setw(spacing) << i->course;
	}
	std::cout << std::endl;
}

void CourseGraph::addEdge(CourseModule &u, CourseModule &v) {
	std::set<vertex>::iterator start = vertices.find(vertex{ u });
	std::set<vertex>::iterator end = vertices.find(vertex{ u });

	if ((start == vertices.end()) || (end == vertices.end())) return;

	std::set<vertex>::iterator test = vertices.begin();
	test->adjacentVertices
}
