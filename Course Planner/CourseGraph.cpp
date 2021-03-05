#include <iostream>
#include <iomanip>
#include "CourseGraph.h"

std::vector<vertex*>::iterator CourseGraph::searchVertices(const CourseModule &c) {
	int start = 0, end = vertices.size() - 1;
	while (start <= end) {
		int mid = (start + end) / 2;
		if (c < vertices.at(mid)->course) end = mid - 1;
		else if (vertices.at(mid)->course < c) start = mid + 1;
		else return vertices.begin() + mid;
	}
	return vertices.end();
}

CourseGraph::CourseGraph() {
	vertices.reserve(20);
}

std::vector<vertex*>::iterator CourseGraph::insert(const CourseModule &c) {
	int start = 0, end = vertices.size() - 1;
	while (start <= end) {
		int mid = (start + end) / 2;
		if (c < vertices.at(mid)->course) end = mid - 1;
		else if (vertices.at(mid)->course < c) start = mid + 1;
		else return vertices.begin() + mid; //Vertex is already in graph so we don't insert
	}

	std::vector<vertex*>::iterator index = vertices.begin() + start;
	vertices.insert(index, new vertex{ CourseModule(c) });
	return searchVertices(c);
}
void CourseGraph::printCourses() const {
	if (vertices.empty()) return;
	int spacing = 15;
	std::cout << std::left;

	for (std::vector<vertex*>::const_iterator i = vertices.begin(); i != vertices.end(); ++i) {
		std::cout << std::setw(spacing) << (*i)->course;
	}
	std::cout << std::endl;


}

void CourseGraph::addEdge(const CourseModule &u, const CourseModule &v) {
	std::vector<vertex*>::iterator start = searchVertices(u);
	if (start == vertices.end()) start = this->insert(u);

	std::vector<vertex*>::iterator end = searchVertices(v);
	if (end == vertices.end()) {
		end = this->insert(v);
		start = searchVertices(u);
	}

	(*start)->edges.push_back(*end);
}
