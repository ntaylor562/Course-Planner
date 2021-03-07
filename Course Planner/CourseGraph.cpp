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

vertex *CourseGraph::search(const CourseModule &c) {
	std::vector<vertex *>::iterator it = searchVertices(c);
	if (it == vertices.end()) return nullptr;
	return *it;
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

void CourseGraph::remove(const CourseModule &c) {
	std::vector<vertex *>::iterator toBeRemoved = searchVertices(c);
	if (toBeRemoved == vertices.end()) return;

	for (auto &i : (*toBeRemoved)->prerequisiteFor) { //Remove this course from other course's prerequisites
		i->prerequisites.remove(*toBeRemoved);
	}
	for (auto &i : (*toBeRemoved)->prerequisites) { //Remove connections to vertices this course is a prerequisite for
		i->prerequisiteFor.remove(*toBeRemoved);
	}

	delete *toBeRemoved;
	vertices.erase(toBeRemoved);
}

bool CourseGraph::empty() {
	return vertices.empty();
}

int CourseGraph::size() {
	return vertices.size();
}

void CourseGraph::printCourses() const {
	if (vertices.empty()) return;
	int spacing = 15;
	std::cout << std::left;

	for (std::vector<vertex*>::const_iterator i = vertices.begin(); i != vertices.end(); ++i) {
		std::cout << std::setw(spacing) << (*i)->course;
	}
	std::cout << std::endl << std::endl;

	for (const auto &i : vertices) {
		std::cout << i->course << std::endl;
		
		if (!i->prerequisites.empty()) {
			std::cout << "Prerequisites: ";
			for (const auto &i : i->prerequisites)
				std::cout << i->course << " ";
			std::cout << std::endl;
		}

		if (!i->prerequisiteFor.empty()) {
			std::cout << "Prerequisite for: ";
			for (const auto &i : i->prerequisiteFor)
				std::cout << i->course << " ";
			std::cout << std::endl;
		}

		std::cout << std::endl;
	}

}

void CourseGraph::addEdge(const CourseModule &u, const CourseModule &v) {
	std::vector<vertex*>::iterator start = searchVertices(u);
	if (start == vertices.end()) start = this->insert(u);

	std::vector<vertex*>::iterator end = searchVertices(v);
	if (end == vertices.end()) {
		end = this->insert(v);
		start = searchVertices(u);
	}
	
	//Return if edge is a duplicate
	if (std::find((*start)->prerequisiteFor.begin(), (*start)->prerequisiteFor.end(), *end) != (*start)->prerequisiteFor.end()) return;
	
	//Add edge
	(*end)->prerequisites.push_back(*start);
	(*start)->prerequisiteFor.push_back(*end);

}

void CourseGraph::removeEdge(const CourseModule &u, const CourseModule &v) {
	std::vector<vertex *>::iterator start = searchVertices(u);
	std::vector<vertex *>::iterator end = searchVertices(v);

	//If u or v do not exist or if there is no edge between them, do nothing
	if (start == vertices.end() || end == vertices.end() || std::find((*end)->prerequisites.begin(), (*end)->prerequisites.end(), *start) == (*end)->prerequisites.end()) return;

	//Remove edge
	(*start)->prerequisiteFor.remove(*end);
	(*end)->prerequisites.remove(*start);
}

std::vector<vertex *>::const_iterator CourseGraph::begin() const {
	return vertices.begin();
}

std::vector<vertex *>::const_iterator CourseGraph::end() const {
	return vertices.end();
}
