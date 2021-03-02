#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include <set>
#include <list>
#include "CourseModule.h"

/**
 * @brief Directed graph used to represent a set of courses where the edges represent prerequisite requirements
 * @details Implemented using an adjacency list
 * @details Adjacent: Two vertices u and v are adjacent if and only if there is a directed edge from u to v
*/
class CourseGraph {
private:

	/**
	 * @brief Holds the data for the actual course and a list of pointers to other nodes that this node is adjacent to
	*/
	struct vertex {
		CourseModule course;
		std::list<vertex *> adjacentVertices;

		bool operator==(const vertex &v) {
			return course == v.course;
		}
		bool operator<(const vertex &v) {
			return course < v.course;
		}
	};

	std::set<vertex> vertices;

public:

	/**
	 * @brief If the course is not already a vertex, create a new vertex with value c
	 * @param c Course being added to the graph
	*/
	void addCourse(const CourseModule &c);

};

#endif