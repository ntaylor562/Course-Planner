#ifndef COURSE_GRAPH_H
#define COURSE_GRAPH_H

#include <vector>
#include <list>
#include "CourseModule.h"

/**
 * @brief Holds the data for the actual course and a list of pointers to other nodes that this node is adjacent to
*/
struct vertex;

/**
 * @brief Directed graph container used to represent a set of courses where the edges represent prerequisite requirements
 * @details Implemented using an adjacency list
 * @details Adjacent: Two vertices u and v are adjacent if and only if there is a directed edge from u to v
*/
class CourseGraph {
private:
	
	/**
	 * @brief Performs a binary search for the vertices in the graph for the given course
	 * @param c Course we're searching for
	 * @return Iterator pointing to the course searched for. Returns vertices.end() if course is not found
	*/
	std::vector<vertex*>::iterator searchVertices(const CourseModule &c);

	std::vector<vertex*> vertices;

public:

	/**
	 * @brief Default constructor to assign capacity of vertices vector
	*/
	CourseGraph();

	/**
	 * @brief Copy constructor to perform deep copy of another graph
	 * @param g Other graph we're copying
	*/
	CourseGraph(const CourseGraph &g);

	/**
	 * @brief Overloaded assignment operator to perform deep copy of another graph. OVERWRITES this graph
	 * @param g Other graph we're copying
	 * @return Reference to this graph
	*/
	CourseGraph &operator=(const CourseGraph &g);

	/*
	 * @brief Destructor to deallocate all vertices created
	*/
	~CourseGraph();

	/**
	 * @brief Uses binary search to find the course entered
	 * @param c Course being searched for
	 * @return Returns a pointer to the vertex representing c in the graph. Returns nullptr if vertex is not in the graph
	*/
	vertex *search(const CourseModule &c);

	/**
	 * @brief If the course is not already a vertex, insert a new vertex with value c
	 * @details If the course is already a vertex, we merge the courses
	 * @param c Course being added to the graph
	 * @return Iterator pointing to the course we inserteda
	*/
	std::vector<vertex*>::iterator insert(const CourseModule &c);

	/**
	 * @brief Merges the data for the course in the graph with the other course given. If course a is not in the graph, nothing is done
	 * @details Incomplete data in one course is filled by the other course's data
	 * @details Conflicts prioritize course a
	 * @details Does not merge prerequisites because course b should not have any since it's not in the graph
	 * @param a Course currently in the graph
	 * @param b The course being merged with the instance in the graph
	*/
	void merge(const CourseModule &a, const CourseModule &b);

	/**
	 * @brief Merges this graph with another one
	 * @details Conflicts prioritize this graph's data
	 * @param g Graph being merged with this graph
	*/
	void merge(const CourseGraph &g);

	/**
	 * @brief Removes course c from the graph
	 * @details If course c is not in the graph, do nothing
	 * @param c Course being removed from the graph
	*/
	void remove(const CourseModule &c);

	/**
	 * @brief Checks if the graph has no vertices
	 * @return Returns true if there are no vertices, false otherwise
	*/
	bool empty() const;

	/**
	 * @brief Used to get the number of vertices in the graph
	 * @return Number of vertices in the graph
	*/
	const int size() const;

	/**
	 * @brief Prints all vertices
	*/
	void printCourses() const;

	/**
	 * @brief Adds a directed edge from u to v
	*/
	void addEdge(const CourseModule &u, const CourseModule &v);

	/**
	 * @brief Removes the edge connecting vertices u and v
	 * @details If there is no edge from u to v, or if u or v does not exist, do nothing
	*/
	void removeEdge(const CourseModule &u, const CourseModule &v);

	/**
	 * @brief Used to iterate through the vertices of the graph
	 * @return An iterator pointing to the beginning of the vertices vector
	*/
	std::vector<vertex *>::const_iterator begin() const;

	/**
	 * @brief Used to iterate through the vertices of the graph
	 * @return An iterator pointing to the end of the vertices vector
	*/
	std::vector<vertex *>::const_iterator end() const;
};

struct vertex {
	CourseModule course;
	
	//Prerequisites for this course (vertices pointing to this course)
	std::list<vertex *> prerequisites; 
	
	//Courses this course is a prerequisite for (vertices this course points to)
	std::list<vertex *> prerequisiteFor; 

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