#ifndef SCHEDULEPREFERENCEDATA_H
#define SCHEDULEPREFERENCEDATA_H

#include "Scheduler.h"
#include "CourseGraph.h"

#define dataPath "data_files/"

/**
 * @brief Used to read and write schedule preference data to a file for future use
 * @details When reading, we give values to a given Scheduler object. When writing, we take in a Scheduler and output the data to a file
*/
class SchedulePreferenceData {
public:
	/**
	 * @brief Reads from the file specified by fileName and populates the reference parameter scheduler
	 * @details Overwrites existing scheduler
	 * @details If file specified does not exist, throws an exception
	 * @param scheduler The scheduler being populated
	 * @param g Course graph the scheduler uses
	 * @param fileName Name of the file we're reading from
	*/
	static void load(Scheduler &scheduler, const CourseGraph &g, std::string fileName) throw (std::runtime_error);

	/**
	 * @brief Prints all the scheduler data into the file so that it can be read next time program is run
	 * @param scheduler Scheduler we're saving data from
	 * @param fileName Name of the file we're saving to
	*/
	static void store(const Scheduler &scheduler, std::string fileName);

};

#endif