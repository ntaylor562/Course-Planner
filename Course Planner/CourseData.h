#ifndef COURSE_DATA_H
#define COURSE_DATA_H

#include <exception>
#include "CourseGraph.h"

#define dataPath "data_files/"

/**
 * @brief Used to read and write course data to a file for future use
 * @details When reading we populate a provided CourseGraph. When writing, we take in a course graph then output the data to the file
*/
class CourseData {
public:
	
	/**
	 * @brief Reads from the file specified by fileName and populates the reference parameter graph
	 * @details Does not overwrite existing graph
	 * @details If any courses match existing courses in the graph, the courses are merged
	 * @details If file specified does not exist, throws an exception
	 * @param graph The graph being populated
	 * @param fileName Name of the file we're reading from
	*/
	static void load(CourseGraph &graph, std::string fileName) throw (std::runtime_error);

	/**
	 * @brief Prints all the course data into the file so that it can be read next time program is run
	 * @param graph Graph we're saving data from
	 * @param fileName Name of the file we're saving to
	*/
	static void store(const CourseGraph &graph, std::string fileName);
};

#endif