#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include <vector>
#include <list>
#include "CourseModule.h"

struct vertex;

/**
 * @brief Directed graph used to represent a set of courses where the edges represent prerequisite requirements
 * @details Implemented using an adjacency list
 * @details Adjacent: Two vertices u and v are adjacent if and only if there is a directed edge from u to v
*/
class CourseGraph {
private:

	

	std::vector<vertex*>::iterator searchVertices(const CourseModule &c);

	std::vector<vertex*> vertices;

public:

	CourseGraph();

	/**
	 * @brief If the course is not already a vertex, insert a new vertex with value c
	 * @param c Course being added to the graph
	 * @return Iterator pointing to the course we inserteda
	*/
	std::vector<vertex*>::iterator insert(const CourseModule &c);

	/**
	 * @brief Prints all vertices
	*/
	void printCourses() const;

	/**
	 * @brief Adds a directed edge from u to v
	*/
	void addEdge(const CourseModule &u, const CourseModule &v);


};

/**
 * @brief Holds the data for the actual course and a list of pointers to other nodes that this node is adjacent to
*/
struct vertex {
	CourseModule course;
	std::list<vertex *> edges;

	bool operator==(const vertex &v) const {
		return course == v.course;
	}
	bool operator<(const vertex &v) const {
		return course < v.course;
	}
	bool operator>(const vertex &v) const {
		return course > v.course;
	}
};

#endif