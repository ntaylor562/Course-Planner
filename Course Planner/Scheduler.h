#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <vector>
#include <set>
#include "CourseModule.h"
#include "CourseGraph.h"


/**
 * @brief Represents a list of courses to be taken in a semester
*/
struct Semester {
	enum Seasons { WINTER, SPRING, SUMMER, FALL };

	Seasons season;
	int year;

	/**
	 * @brief The maximum number of units to take in this semester
	 * @details When maxUnits != 0, the maximum units for the schedule generator is overridden with this value
	*/
	int maxUnits = 0;

	/**
	 * @brief Courses being taken this semester
	*/
	std::vector<CourseModule> courses;

	/**
	 * @brief A vector containing courses that we cannot take in this semester
	*/
	std::vector<CourseModule> restricted;


	const bool operator<(const Semester &s) const {
		if (year < s.year) return true;
		if (season < s.season) return true;
		return false;
	}
	const bool operator>(const Semester &s) const {
		if (year > s.year) return true;
		if (season > s.season) return true;
		return false;
	}
	const bool operator==(const Semester &s) const {
		return season == s.season && year == s.year;
	}
};

/**
 * @brief Represents all semesters of courses planned out contained in a vector of Semesters
*/
struct Schedule {
	std::vector<Semester> CoursePlan;
};


/**
 * @brief Used to generate schedules and allows for user to put restrictions on what courses can be taken when
 * @details Spring and Fall courses are automatically allowed while Winter and Summer are automatically not allowed. This can be changed with the setters
*/
class Scheduler {
private:

	CourseGraph Courses;
	std::set<CourseModule> completedCourses;

	std::vector<Semester> semsWithRestrictions;
	
	bool winterAllowed, springAllowed, summerAllowed, fallAllowed;

	int maxUnits;

	//Sets s to the next season allowed and if the season has to go to the next year, the year is incremented
	//Does not check whether any of the seasons are allowed. If that is the case, there will be an infinite loop
	void incrementSeason(Semester::Seasons &s, int &y);

public:

	/**
	 * @brief Initializes the scheduler with a graph containing the courses and a list of completed courses
	 * @param g Graph containing all courses that need to be taken or have already been taken
	 * @param completed Vector containing all courses that have already been taken and don't need to be planned
	*/
	Scheduler(const CourseGraph &g, const std::set<CourseModule> &completed = std::set<CourseModule>());

	/**
	 * @brief Marks the course given as complete by removing it from the graph and adding it to the list of completed courses
	 * @param c Course completed
	*/
	void complete(const CourseModule &c);

	/**
	 * @brief Marks the courses given as complete by removing them from the graph and adding them to the list of completed courses
	 * @param vect Vector containing the courses we're marking as complete
	*/
	void complete(const std::vector<CourseModule> &vect);

	/**
	 * @brief Allows user to disallow a course from being taken in a particular semester
	 * @param c Course being restricted
	 * @param season Which semester of the year being restricted (Ex: Spring, Fall, etc.)
	 * @param year Year of the semester
	*/
	void addRestriction(const CourseModule &c, Semester::Seasons season, int year);

	/**
	 * @brief Removes a restriction for a given semester
	 * @param c The restricted course
	 * @param season Which semester of the year was restricted
	 * @param year Year of the semester
	*/
	void removeRestriction(const CourseModule &c, Semester::Seasons season, int year);

	/**
	 * @brief Sets a unit limit for a given semester
	 * @details Minimum = 1
	 * @param lim The maximum units to be taken in the given semester
	 * @param season Which semester of the year being limited (Ex: Spring, Fall, etc.)
	 * @param year Year of the semester
	*/
	void setUnitLimit(int lim, Semester::Seasons season, int year);

	/**
	 * @brief Sets an overall unit limit (default = 15)
	 * @details Minimum = 1
	 * @param lim The maximum units to be taken in any semester
	*/
	void setUnitLimit(int lim);

	Schedule generateSchedule(Semester::Seasons currentSeason, int currentYear);

	//Lets user determine if they want to take courses in particular semesters

	void setWinterAllowed(bool allowed);
	void setSpringAllowed(bool allowed);
	void setSummerAllowed(bool allowed);
	void setFallAllowed(bool allowed);

	std::vector<Semester::Seasons> getRestrictedSeasons();

	std::vector<Semester> getRestrictedSemesters();
	
	int getMaxUnits();

};

#endif