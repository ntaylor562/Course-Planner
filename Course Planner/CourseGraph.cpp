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

CourseGraph::CourseGraph(const CourseGraph &g) {
	for (auto &i : vertices) { //Deallocating this graph
		delete i;
	}
	vertices = std::vector<vertex *>(g.size());
	for (int i = 0; i < g.size(); ++i) {
		vertices[i] = new vertex{ g.vertices[i]->course }; //Copy course from each vertex but not the edges
	}

	//Adding edges using pointers to vertices of this graph
	for (const auto &gVertex : g) {
		for (const auto &c : gVertex->prerequisiteFor) {
			addEdge(gVertex->course, c->course);
		}
	}
}

CourseGraph &CourseGraph::operator=(const CourseGraph &g) {
	if (this == &g) return *this;

	for (auto &i : vertices) { //Deallocating this graph
		delete i;
	}
	vertices = std::vector<vertex *>(g.size());
	for (int i = 0; i < g.size(); ++i) {
		vertices[i] = new vertex{ g.vertices[i]->course }; //Copy course from each vertex but not the edges
	}

	//Adding edges using pointers to vertices of this graph
	for (const auto &gVertex : g) {
		for (const auto &c : gVertex->prerequisiteFor) {
			addEdge(gVertex->course, c->course);
		}
	}

	return *this;
}

CourseGraph::~CourseGraph() {
	for (auto &i : vertices)
		delete i;
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
		else { //Vertex is already in graph so we merge the vertices
			merge(c);
			return vertices.begin() + mid;
		}
	}

	std::vector<vertex*>::iterator index = vertices.begin() + start;
	vertices.insert(index, new vertex{ CourseModule(c) });
	return searchVertices(c);
}

std::vector<vertex *>::iterator CourseGraph::insert(const vertex &v) {
	vertex *originalVertex= *insert(v.course);
	//Adding prerequisites
	for (const auto &i : v.prerequisites) {
		insert(i->course);
		addEdge(i->course, originalVertex->course);
	}
	//Adding prerequisiteFor
	for (const auto &i : v.prerequisiteFor) {
		insert(i->course);
		addEdge(originalVertex->course, i->course);
	}
	return searchVertices(originalVertex->course);
}

void CourseGraph::merge(const CourseModule &c) {
	vertex *v = CourseGraph::search(c);

	if (v == nullptr) return;

	CourseModule *result = &v->course;
	if (result->getUnits() == 0) result->setUnits(c.getUnits());
	if (result->getCourseTitle() == "") result->setCourseTitle(c.getCourseTitle());
	if (result->getDescription() == "") result->setDescription(c.getDescription());

}

void CourseGraph::merge(const vertex *v) {
	//Vertex in this graph that corresponds to v
	vertex *originalVertex = CourseGraph::search(v->course);

	//No vertex in the graph to merge with
	if (originalVertex == nullptr) return;

	//Merge course data
	merge(v->course);

	//Merging prerequisites
	for (const auto &i : v->prerequisites) {
		insert(i->course);
		addEdge(i->course, originalVertex->course);
	}
	//Merging prerequisiteFor
	for (const auto &i : v->prerequisiteFor) {
		insert(i->course);
		addEdge(originalVertex->course, i->course);
	}
}

void CourseGraph::merge(const CourseGraph &g) {
	for (const auto &i : g) {
		insert(i->course);
	}

	for (const auto &i : g) {
		for (const auto &p : i->prerequisites) {
			addEdge(p->course, i->course);
		}
	}
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

bool CourseGraph::empty() const {
	return vertices.empty();
}

const int CourseGraph::size() const {
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
	//Return if u and v are the same course because no loops are allowed
	if (u == v) return;

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
